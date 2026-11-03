#include "main.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

namespace kp::path_recorder {
    namespace {
        using kp::pathrec::Item;
        using kp::pathrec::Kind;
        using kp::pathrec::Routine;

        constexpr auto POINT = pros::E_CONTROLLER_DIGITAL_A;
        constexpr auto UNDO = pros::E_CONTROLLER_DIGITAL_X;
        constexpr auto SAVE = pros::E_CONTROLLER_DIGITAL_Y;
        constexpr auto DISCARD_SHIFT = pros::E_CONTROLLER_DIGITAL_DOWN;

        pros::Controller master(pros::E_CONTROLLER_MASTER);

        // Where the robot and mechanisms stand after a routine: the reference the next
        // point is compared against. Rebuilt from scratch after an UNDO.
        struct State {
            float x = 0, y = 0, theta = 0;
            float lift = 0;
            int claw = 0, toggler = 0;
        };

        State state_after(const Routine& r, const State& init) {
            State s = init;
            for (const Item& it : r) {
                switch (it.kind) {
                    case Kind::START:
                    case Kind::MOVE:
                    case Kind::POSE:
                        s.x = it.x;
                        s.y = it.y;
                        s.theta = it.theta;
                        break;
                    case Kind::TURN: s.theta = it.theta; break;
                    case Kind::LIFT: s.lift = it.deg; break;
                    case Kind::BUMP: s.lift += (float)kp::lift::BUMP_DEG; break;
                    case Kind::CLAW: s.claw = it.a; break;
                    case Kind::TOGGLE: s.toggler = it.a; break;
                    default: break;
                }
            }
            return s;
        }

        int count_points(const Routine& r) {
            int n = 0;
            for (const Item& it : r) {
                if (kp::pathrec::is_drive(it.kind)) n++;
            }
            return n;
        }

        // Keeps the robot responsive while waiting, so a stick command can't get stuck on.
        void live_wait(int ms) {
            const uint32_t start = pros::millis();
            while (pros::millis() - start < (uint32_t)ms) {
                kp::teleop::tick();
                pros::delay(20);
            }
        }

        // The controller screen takes about ten prints a second and garbles past that, so
        // one row is redrawn per pass. Rows are padded so short lines overwrite long ones.
        class Screen {
        public:
            void status(const char* text) {
                std::snprintf(status_, sizeof(status_), "%s", text);
                status_until_ = pros::millis() + 1500;
            }

            void draw(int points, int steps) {
                const uint32_t now = pros::millis();
                if (now - last_ < 100) return;
                last_ = now;
                switch (row_) {
                    case 0: master.print(0, 0, "REC pt%-3d i%-4d", points, steps); break;
                    case 1: master.print(1, 0, "%-18s", now < status_until_ ? status_ : ""); break;
                    default: {
                        const lemlib::Pose p = kp::chassis::current_pose();
                        master.print(2, 0, "%-5.0f %-5.0f %-5.0f", p.x, p.y, p.theta);
                        break;
                    }
                }
                row_ = (row_ + 1) % 3;
            }

        private:
            char status_[20] = "";
            uint32_t status_until_ = 0;
            uint32_t last_ = 0;
            int row_ = 0;
        };

        void push(Routine& r, Kind kind, int a = 0, int t = 0, float deg = 0) {
            Item it;
            it.kind = kind;
            it.a = a;
            it.t = t;
            it.deg = deg;
            r.push_back(it);
        }
    }

    void run() {
        kp::controller::pause_screen();

        const lemlib::Pose p0 = kp::chassis::current_pose();
        if (!std::isfinite(p0.x) || !std::isfinite(p0.y) || !std::isfinite(p0.theta)) {
            master.print(0, 0, "REC: ODOM BAD     ");
            pros::delay(60);
            master.rumble("---");
            pros::delay(1000);
            kp::controller::resume_screen();
            return;
        }

        Routine r;
        Item start;
        start.kind = Kind::START;
        start.x = (float)p0.x;
        start.y = (float)p0.y;
        start.theta = (float)p0.theta;
        r.push_back(start);

        State init;
        const double lp = kp::lift::get_position();
        init.lift = std::isfinite(lp) ? (float)lp : 0.0f;
        init.claw = kp::claw::is_extended() ? 1 : 0;
        init.toggler = kp::toggle::is_on() ? 1 : 0;
        State st = state_after(r, init);
        std::vector<std::size_t> points; // where each point's steps begin in r
        int bumps_seen = kp::lift::bump_count();

        Screen screen;
        screen.status("A=pt X=undo Y=save");
        master.rumble(".");

        master.get_digital_new_press(UNDO); // drop stale edges
        master.get_digital_new_press(POINT);

        bool save = true;
        while (true) {
            kp::teleop::tick();

            if (master.get_digital_new_press(SAVE)) {
                save = !master.get_digital(DISCARD_SHIFT);
                break;
            }

            if (master.get_digital_new_press(POINT)) {
                // Mid-bump the lift height is meaningless, so let the bump finish first.
                while (kp::lift::bump_active()) {
                    kp::teleop::tick();
                    pros::delay(20);
                }

                const lemlib::Pose p = kp::chassis::current_pose();
                const double lift_now = kp::lift::get_position();
                const int claw_now = kp::claw::is_extended() ? 1 : 0;
                const int toggler_now = kp::toggle::is_on() ? 1 : 0;

                // A bump happens AFTER the claw closes, so it can't be folded into the
                // lift move that runs before the drive: replay it as its own step.
                const int count_now = kp::lift::bump_count();
                const int bumps = count_now - bumps_seen;
                const double lift_target = lift_now - bumps * kp::lift::BUMP_DEG;

                Item prev;
                prev.x = st.x;
                prev.y = st.y;
                prev.theta = st.theta;
                const Item drive = kp::pathrec::classify_node(prev, (float)p.x, (float)p.y, (float)p.theta);
                const bool moved = kp::pathrec::is_drive(drive.kind);
                const bool lifted = std::isfinite(lift_target) &&
                                    std::fabs(lift_target - st.lift) > kp::pathrec::LIFT_EPS;
                const bool clawed = claw_now != st.claw;
                const bool toggled = toggler_now != st.toggler;

                if (!moved && !lifted && !clawed && !toggled && bumps == 0) {
                    screen.status("NOTHING CHANGED");
                    master.rumble("..");
                } else {
                    points.push_back(r.size());
                    if (lifted) push(r, Kind::LIFT, 0, 0, (float)lift_target); // travels during the drive
                    if (moved) r.push_back(drive);
                    if (lifted && (clawed || !moved)) push(r, Kind::WAITLIFT, 0, kp::pathrec::WAITLIFT_MS);
                    if (toggled) push(r, Kind::TOGGLE, toggler_now);
                    if (clawed) push(r, Kind::CLAW, claw_now);
                    for (int i = 0; i < bumps; i++) push(r, Kind::BUMP);
                    bumps_seen = count_now;
                    st = state_after(r, init);

                    char msg[20];
                    std::snprintf(msg, sizeof(msg), "PT%d %s%s%s%s%s", count_points(r),
                                  moved ? kp::pathrec::tag_of(drive.kind) : "", lifted ? " +L" : "",
                                  toggled ? " +T" : "", clawed ? (claw_now ? " +G" : " +R") : "",
                                  bumps > 0 ? " +B" : "");
                    screen.status(msg);
                    master.rumble(".");
                }
            }

            if (master.get_digital_new_press(UNDO)) {
                if (!points.empty()) {
                    r.erase(r.begin() + (long)points.back(), r.end());
                    points.pop_back();
                    // The lift stays physically where the undone bump left it, so the next
                    // point records that height as a LIFT instead of a second BUMP.
                    bumps_seen = kp::lift::bump_count();
                    st = state_after(r, init);
                    char msg[20];
                    std::snprintf(msg, sizeof(msg), "UNDO -> %d pts", count_points(r));
                    screen.status(msg);
                    master.rumble(".");
                } else {
                    screen.status("NOTHING TO UNDO");
                }
            }

            screen.draw(count_points(r), (int)r.size());
            pros::delay(20);
        }

        kp::chassis::stop();

        if (!save) {
            master.print(0, 0, "%-18s", "DISCARDED");
            live_wait(60);
            master.rumble("..");
        } else {
            kp::pathrec::remember(r); // replayable until power-off even with no SD card
            const int idx = kp::pathrec::save_autonumbered(r, "recorded");
            kp::pathrec::dump_cpp(r); // to `pros terminal`: the lasting copy with no card
            if (idx >= 0) {
                master.print(0, 0, "SAVED path_%02d     ", idx);
                live_wait(60);
                master.rumble("-");
            } else {
                master.print(0, 0, "%-18s", "NO SD: in memory");
                live_wait(60);
                master.rumble("...");
            }
        }
        live_wait(110);
        master.print(1, 0, "%-18s", "");
        live_wait(110);
        master.print(2, 0, "%-18s", "");
        live_wait(1000);

        master.get_digital_new_press(DISCARD_SHIFT); // so the setup screen doesn't see it as a press
        kp::controller::resume_screen();
    }

    void dump_saved() {
        Routine r;
        if (kp::pathrec::load_default(r) && !r.empty()) {
            kp::pathrec::dump_cpp(r);
            CTRL_VIBRATE("-")
        } else {
            printf("[recorder] no saved path\n");
            CTRL_VIBRATE("..")
        }
    }
}
