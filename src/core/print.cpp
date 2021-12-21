#include "core.h"

#include <stdio.h>

namespace core
{

i32 PrintF(constptr char* _format, ...)
{
    va_list argptr;
    va_start(argptr, _format);
    i32 ret = vfprintf(stdout, _format, argptr); // pass the arguments to vfprintf
    va_end(argptr);
    return ret;
}

i32 PrintErrF(constptr char* _format, ...)
{
    va_list argptr;
    va_start(argptr, _format);
    i32 ret = vfprintf(stderr, _format, argptr); // pass the arguments to vfprintf
    va_end(argptr);
    return ret;
}

i32 SprintF(modptr char* _s, constptr char* _format, ...)
{
    va_list argptr;
    va_start(argptr, _format);
    i32 ret = sprintf(_s, _format, argptr); // pass the arguments to vfprintf
    va_end(argptr);
    return ret;
}

} // namespace core