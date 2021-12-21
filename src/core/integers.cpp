#include "core.h"

namespace core
{

i32 DigitCountI32(i32 _n) { return DigitCountI64((i64)_n); }
i32 DigitCountI64(i64 _n)
{
    i32 ret = 0;
    while (_n != 0) {
        _n /= 10;
        ret++;
    }
    return ret;
}

} // namespace core