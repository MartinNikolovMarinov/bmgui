#include "core.h"

#include <stdlib.h>
#include <time.h>

namespace core
{

void InitRand()
{
    srand(time(NULL));
}

i32 RandI32(i32 _min, i32 _max)
{
    return _min + rand() % _max;
}

global_variable const char alphaChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
global_variable const i32 alphaCharsLen = sizeof(alphaChars) - 1; // alphaChars includes terminating '\0'

void RandRawStr(modptr char* _data, i32 _len)
{
    i32 i;
    for (i = 0; i < _len; i++) {
        i32 random_index = (f64)rand() / RAND_MAX * alphaCharsLen;
        _data[i] = alphaChars[random_index];
    }
    _data[i] = '\0';
}

} // namespace core
