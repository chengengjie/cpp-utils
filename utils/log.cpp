#include "log.h"

#include <iomanip>
#include <sstream>

using namespace std;

timer::timer() { start(); }
void timer::start() { _start = clock::now(); }
double timer::elapsed() const { return std::chrono::duration<double>(clock::now() - _start).count(); }

timer tstamp;

ostream& operator<<(ostream& os, const timer& t) {
    os << "[" << setprecision(3) << setw(8) << fixed << t.elapsed() << "] ";
    return os;
}
string to_string(const timer& t) {
    ostringstream oss;
    oss << t;
    return oss.str();
}

ostream& log(ostream& os) {
    os << to_string(tstamp);
    return os;
}