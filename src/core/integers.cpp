#include "core.h"

namespace core
{

bool8 IsInRange(u32 _min, u32 _n, u32 _max)    { return (_min < _n && _n < _max); }
bool8 IsInRangeInc(u32 _min, u32 _n, u32 _max) { return (_min <= _n && _n <= _max); }
bool8 IsInRange(i32 _min, i32 _n, i32 _max)    { return (_min < _n && _n < _max); }
bool8 IsInRangeInc(i32 _min, i32 _n, i32 _max) { return (_min <= _n && _n <= _max); }

i32 DigitCountI32(i32 _n) { return DigitCountI64((i64)_n); }
i32 DigitCountI64(i64 _n)
{
    Assert(_n >= 0);
    i32 ret = 0;
    while (_n != 0) {
        _n /= 10;
        ret++;
    }
    return ret;
}

} // namespace core