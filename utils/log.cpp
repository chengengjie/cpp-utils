#include "log.h"

#include <iomanip>
#include <sstream>

namespace utils {

timer::timer() { start(); }
void timer::start() { _start = clock::now(); }
double timer::elapsed() const { return std::chrono::duration<double>(clock::now() - _start).count(); }

timer tstamp;

std::ostream& operator<<(std::ostream& os, const timer& t) {
    std::ostringstream oss; // seperate the impact of format
    oss << "[" << std::setprecision(3) << std::setw(8) << std::fixed << t.elapsed() << "] ";
    os << oss.str();
    return os;
}

std::ostream& log(std::ostream& os) {
    os << tstamp;
    return os;
}

}