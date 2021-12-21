#include "core.h"

namespace core
{

bool8 FirstNBitsAre(u8 _v, u8 _n, u8 _bits) { return (_v >> (sizeof(u8) * 8 - _n)) == _bits; }

} // namespace core
