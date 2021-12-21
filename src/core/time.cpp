#include "core.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include <time.h>

namespace core
{

u64 GetTimeNowMS()
{
    // TODO: document this. Raw documentation for timespec_get:
    // ts->tv_sec is set to the number of seconds since an implementation defined epoch, truncated to a whole value
    // ts->tv_nsec member is set to the integral number of nanoseconds, rounded to the resolution of the system clock
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return ((u64) now.tv_sec) * MICROSECOND + ((u64) now.tv_nsec) / MILLISECOND;
}

void ThreadSleep(u64 ms)
{
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}

} // namespace core
