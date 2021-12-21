#include "core.h"

namespace core
{

i64 PowI32(i32 _n, i32 _power) { return (i64)PowF64((f64)_n, _power); }
i64 PowI64(i64 _n, i32 _power) { return (i64)PowF64(_n, _power);      }
f32 PowF32(f32 _n, i32 _power) { return (f32)PowF64((f64)_n, _power); }
f64 PowF64(f64 _n, i32 _power)
{
    if (_power < 0) {
        _power = -_power;
        _n = 1/_n;
    }

    f64 res = 1;
    for (i32 i = 0; i < _power; i++)
    {
        res = res * _n;
    }

    return res;
}

} // namespace core