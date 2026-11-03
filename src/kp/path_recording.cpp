#include "main.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

// play() and dump_cpp() map each step the same way; keep them in sync.
namespace kp::pathrec {
    namespace {
        float wrap180(float a) {
            while (a > 180.0f) a -= 360.0f;
            while (a < -180.0f) a += 360.0f;
            return a;
        }

        int drive_timeout(float dist) {
            return std::clamp((int)(dist * MS_PER_INCH) + DRIVE_MS_MIN, DRIVE_MS_MIN, DRIVE_MS_MAX);
        }

        int turn_timeout(float deg) {
            return std::clamp((int)(deg * MS_PER_DEG) + TURN_MS_MIN, TURN_MS_MIN, TURN_MS_MAX);
        }

        // At the top of every saved file, so whoever edits it has the format in front of them.
        const char* const FILE_HEADER =
            "# Recorded path. One step per line, run top to bottom. Edit freely, save,\n"
            "# then pick \"recorded\" as the auton. # lines are ignored.\n"
            "#   START x y theta        set the pose (inches, inches, degrees)\n"
            "#   MOVE x y ms fwd        drive to a point; fwd 1 = forwards, 0 = backing up\n"
            "#   POSE x y theta ms fwd  drive to a point AND end facing theta\n"
            "#   TURN theta ms          turn in place to theta\n"
            "#   LIFT deg               send the lift to deg (moves during the next drive)\n"
            "#   WAITLIFT ms            wait for the lift to get there\n"
            "#   CLAW 1|0               1 = close, 0 = open\n"
            "#   BUMP                   the same lift bump R1 does\n"
            "#   TOGGLE 1|0             toggler: 1 = on, 0 = off\n"
            "#   WAIT ms                pause\n"
            "# ms on a drive is a timeout: the move ends early when it arrives.\n"
            "# Lift degrees count from the bottom, so start with the lift DOWN.\n";

        // Built on first use rather than during static init.
        Routine& remembered() {
            static Routine r;
            return r;
        }

        std::string num(float v) {
            char b[24];
            std::snprintf(b, sizeof(b), "%g", v);
            return b;
        }

        std::string drive_params(const Item& it) {
            char b[64];
            std::snprintf(b, sizeof(b), "{%s.maxSpeed = %d}", it.a ? "" : ".forwards = false, ",
                          DRIVE_MAX_SPEED);
            return b;
        }
    }

    bool is_drive(Kind k) {
        return k == Kind::MOVE || k == Kind::POSE || k == Kind::TURN;
    }

    const char* tag_of(Kind k) {
        switch (k) {
            case Kind::START: return "START";
            case Kind::MOVE: return "MOVE";
            case Kind::POSE: return "POSE";
            case Kind::TURN: return "TURN";
            case Kind::LIFT: return "LIFT";
            case Kind::WAITLIFT: return "WAITLIFT";
            case Kind::CLAW: return "CLAW";
            case Kind::BUMP: return "BUMP";
            case Kind::TOGGLE: return "TOGGLE";
            case Kind::WAIT: return "WAIT";
        }
        return "WAIT";
    }

    Item classify_node(const Item& prev, float x, float y, float theta) {
        const float dx = x - prev.x, dy = y - prev.y;
        const float dist = std::sqrt(dx * dx + dy * dy);
        const float dtheta = wrap180(theta - prev.theta);

        Item it;
        if (dist < POS_EPS) {
            if (std::fabs(dtheta) < THETA_EPS) { // below both thresholds: not a motion
                it.kind = Kind::WAIT;
                it.t = 0;
            } else {
                it.kind = Kind::TURN;
                it.theta = theta;
                it.t = turn_timeout(std::fabs(dtheta));
            }
            return it;
        }

        // LemLib heading: 0 = +Y, CW positive -> atan2(dx, dy), not the usual (dy, dx).
        const float travel = wrap180(std::atan2(dx, dy) * 180.0f / (float)M_PI);
        // Backed up if the travel direction points away from where it was facing.
        const bool reversed = std::fabs(wrap180(travel - prev.theta)) > 90.0f;
        // Where a plain drive along the line leaves the robot facing. Comparing against the
        // travel direction alone would make every straight reverse look like a turn.
        const float line_heading = reversed ? wrap180(travel + 180.0f) : travel;

        it.kind = std::fabs(wrap180(theta - line_heading)) >= THETA_EPS ? Kind::POSE : Kind::MOVE;
        it.x = x;
        it.y = y;
        // POSE replays it. MOVE doesn't, but the recorder compares the next point against it.
        it.theta = theta;
        it.t = drive_timeout(dist);
        it.a = reversed ? 0 : 1;
        return it;
    }

    bool save(const Routine& r, const char* path, const char* name) {
        if (!pros::usd::is_installed()) return false;
        FILE* f = std::fopen(path, "w");
        if (!f) return false;

        std::fputs(FILE_HEADER, f);
        std::fprintf(f, "NAME %s\n", name ? name : "");
        for (const Item& it : r) {
            const char* tag = tag_of(it.kind);
            switch (it.kind) {
                case Kind::START: std::fprintf(f, "%s %.2f %.2f %.2f\n", tag, it.x, it.y, it.theta); break;
                case Kind::MOVE: std::fprintf(f, "%s %.2f %.2f %d %d\n", tag, it.x, it.y, it.t, it.a); break;
                case Kind::POSE:
                    std::fprintf(f, "%s %.2f %.2f %.2f %d %d\n", tag, it.x, it.y, it.theta, it.t, it.a);
                    break;
                case Kind::TURN: std::fprintf(f, "%s %.2f %d\n", tag, it.theta, it.t); break;
                case Kind::LIFT: std::fprintf(f, "%s %.1f\n", tag, it.deg); break;
                case Kind::CLAW:
                case Kind::TOGGLE: std::fprintf(f, "%s %d\n", tag, it.a); break;
                case Kind::BUMP: std::fprintf(f, "%s\n", tag); break;
                case Kind::WAITLIFT:
                case Kind::WAIT: std::fprintf(f, "%s %d\n", tag, it.t); break;
            }
        }
        std::fclose(f);
        return true;
    }

    int save_autonumbered(const Routine& r, const char* name) {
        if (!pros::usd::is_installed()) return -1;

        int n = 1;
        if (FILE* rf = std::fopen(INDEX_PATH, "r")) {
            if (std::fscanf(rf, "%d", &n) != 1 || n < 1) n = 1;
            std::fclose(rf);
        }
        if (n > 99) n = 1; // ARCHIVE_FMT is two digits: wrap rather than overflow
        if (FILE* wf = std::fopen(INDEX_PATH, "w")) {
            std::fprintf(wf, "%d\n", n + 1);
            std::fclose(wf);
        }

        char path[32];
        std::snprintf(path, sizeof(path), ARCHIVE_FMT, n);
        if (!save(r, path, name)) return -1;
        if (!save(r, PLAY_PATH, name)) return -1;
        return n;
    }

    void remember(const Routine& r) {
        remembered() = r;
    }

    bool load(Routine& out, const char* path) {
        if (!pros::usd::is_installed()) return false;
        FILE* f = std::fopen(path, "r");
        if (!f) return false;

        out.clear();
        char line[128];
        while (std::fgets(line, sizeof(line), f)) {
            char tag[16];
            int used = 0;
            // %n: where the keyword ended, even with leading spaces in a hand edit.
            if (std::sscanf(line, " %15s%n", tag, &used) != 1 || tag[0] == '#') continue;
            const char* args = line + used;

            Item it;
            if (std::strcmp(tag, "START") == 0) {
                it.kind = Kind::START;
                if (std::sscanf(args, "%f %f %f", &it.x, &it.y, &it.theta) != 3) continue;
            } else if (std::strcmp(tag, "MOVE") == 0) {
                it.kind = Kind::MOVE;
                it.a = 1; // a hand-written line may leave off fwd: forwards
                if (std::sscanf(args, "%f %f %d %d", &it.x, &it.y, &it.t, &it.a) < 3) continue;
            } else if (std::strcmp(tag, "POSE") == 0) {
                it.kind = Kind::POSE;
                it.a = 1;
                if (std::sscanf(args, "%f %f %f %d %d", &it.x, &it.y, &it.theta, &it.t, &it.a) < 4) continue;
            } else if (std::strcmp(tag, "TURN") == 0) {
                it.kind = Kind::TURN;
                if (std::sscanf(args, "%f %d", &it.theta, &it.t) != 2) continue;
            } else if (std::strcmp(tag, "LIFT") == 0) {
                it.kind = Kind::LIFT;
                if (std::sscanf(args, "%f", &it.deg) != 1) continue;
            } else if (std::strcmp(tag, "WAITLIFT") == 0) {
                it.kind = Kind::WAITLIFT;
                if (std::sscanf(args, "%d", &it.t) != 1) continue;
            } else if (std::strcmp(tag, "CLAW") == 0) {
                it.kind = Kind::CLAW;
                if (std::sscanf(args, "%d", &it.a) != 1) continue;
            } else if (std::strcmp(tag, "BUMP") == 0) {
                it.kind = Kind::BUMP;
            } else if (std::strcmp(tag, "TOGGLE") == 0) {
                it.kind = Kind::TOGGLE;
                if (std::sscanf(args, "%d", &it.a) != 1) continue;
            } else if (std::strcmp(tag, "WAIT") == 0) {
                it.kind = Kind::WAIT;
                if (std::sscanf(args, "%d", &it.t) != 1) continue;
            } else {
                continue; // NAME and anything unknown: skip
            }
            out.push_back(it);
        }
        std::fclose(f);
        return true;
    }

    bool load_default(Routine& out) {
        if (load(out, PLAY_PATH) && !out.empty()) return true;
        for (int i = 99; i >= 1; i--) {
            char path[32];
            std::snprintf(path, sizeof(path), ARCHIVE_FMT, i);
            if (load(out, path) && !out.empty()) return true;
        }
        if (!remembered().empty()) {
            out = remembered();
            return true;
        }
        return false;
    }

    void play(const Routine& r) {
        lemlib::Chassis* chassis = kp::chassis::get_chassis();
        for (const Item& it : r) {
            switch (it.kind) {
                case Kind::START:
                    chassis->setPose(it.x, it.y, it.theta);
                    break;
                case Kind::MOVE:
                    chassis->moveToPoint(it.x, it.y, it.t,
                                         {.forwards = it.a != 0, .maxSpeed = (float)DRIVE_MAX_SPEED}, false);
                    break;
                case Kind::POSE:
                    chassis->moveToPose(it.x, it.y, it.theta, it.t,
                                        {.forwards = it.a != 0, .maxSpeed = (float)DRIVE_MAX_SPEED}, false);
                    break;
                case Kind::TURN:
                    chassis->turnToHeading(it.theta, it.t, {.maxSpeed = TURN_MAX_SPEED}, false);
                    break;
                case Kind::LIFT:
                    kp::lift::go_to(it.deg);
                    break;
                case Kind::WAITLIFT:
                    kp::lift::wait_until_there(it.t);
                    break;
                case Kind::CLAW:
                    kp::claw::set(it.a != 0);
                    pros::delay(CLAW_SETTLE_MS);
                    break;
                case Kind::BUMP:
                    kp::lift::bump_up();
                    break;
                case Kind::TOGGLE:
                    kp::toggle::set(it.a != 0);
                    break;
                case Kind::WAIT:
                    pros::delay(it.t);
                    break;
            }
        }
    }

    void dump_cpp(const Routine& r, const char* fn_name) {
        std::printf("// ---- paste into src/automation/test.cpp, declare it in test.hpp,\n");
        std::printf("// ---- and add it with kp::automation::set in initialize() ----\n");
        std::printf("void %s() {\n", fn_name);
        std::printf("    lemlib::Chassis* chassis = kp::chassis::get_chassis();\n");
        for (const Item& it : r) {
            switch (it.kind) {
                case Kind::START:
                    std::printf("    chassis->setPose(%s, %s, %s);\n", num(it.x).c_str(), num(it.y).c_str(),
                                num(it.theta).c_str());
                    break;
                case Kind::MOVE:
                    std::printf("    chassis->moveToPoint(%s, %s, %d, %s, false);\n", num(it.x).c_str(),
                                num(it.y).c_str(), it.t, drive_params(it).c_str());
                    break;
                case Kind::POSE:
                    std::printf("    chassis->moveToPose(%s, %s, %s, %d, %s, false);\n", num(it.x).c_str(),
                                num(it.y).c_str(), num(it.theta).c_str(), it.t, drive_params(it).c_str());
                    break;
                case Kind::TURN:
                    std::printf("    chassis->turnToHeading(%s, %d, {.maxSpeed = %d}, false);\n",
                                num(it.theta).c_str(), it.t, TURN_MAX_SPEED);
                    break;
                case Kind::LIFT:
                    std::printf("    kp::lift::go_to(%s);\n", num(it.deg).c_str());
                    break;
                case Kind::WAITLIFT:
                    std::printf("    kp::lift::wait_until_there(%d);\n", it.t);
                    break;
                case Kind::CLAW:
                    std::printf("    kp::claw::set(%s);\n    pros::delay(%d);\n", it.a ? "true" : "false",
                                CLAW_SETTLE_MS);
                    break;
                case Kind::BUMP:
                    std::printf("    kp::lift::bump_up();\n");
                    break;
                case Kind::TOGGLE:
                    std::printf("    kp::toggle::set(%s);\n", it.a ? "true" : "false");
                    break;
                case Kind::WAIT:
                    std::printf("    pros::delay(%d);\n", it.t);
                    break;
            }
        }
        std::printf("}\n");
    }

    void play_recorded() {
        Routine r;
        if (load_default(r) && !r.empty()) {
            play(r);
        }
    }
}
