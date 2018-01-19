#include "log.h"

#include <iomanip>
#include <sstream>

#if defined(__unix__)
#include <unistd.h>
#include <sys/resource.h>
#elif defined(_WIN32)
#include <windows.h>
#include <psapi.h>
#endif

namespace utils {

timer::timer() { start(); }
void timer::start() { _start = clock::now(); }
double timer::elapsed() const { return std::chrono::duration<double>(clock::now() - _start).count(); }

timer tstamp;

std::ostream& operator<<(std::ostream& os, const timer& t) {
    std::ostringstream oss;  // seperate the impact of format
    oss << "[" << std::setprecision(3) << std::setw(8) << std::fixed << t.elapsed() << "] ";
    os << oss.str();
    return os;
}

double mem_use::get() {
#if defined(__unix__)
    long rss = 0L;
    FILE* fp = NULL;
    if ((fp = fopen("/proc/self/statm", "r")) == NULL) {
        return 0; /* Can't open? */
    }
    if (fscanf(fp, "%*s%ld", &rss) != 1) {
        fclose(fp);
        return 0; /* Can't read? */
    }
    fclose(fp);
    return rss * sysconf(_SC_PAGESIZE) / 1048576.0;

#elif defined(_WIN32)
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return info.WorkingSetSize / 1048576.0;
#endif
}

std::ostream& log(std::ostream& os) {
    os << tstamp;
    return os;
}

}  // namespace utils