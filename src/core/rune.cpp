#include "core.h"

namespace core
{

namespace core_internal {
    const u32 UTF8_TWO_BYTE_MASK   = 0b11000000;
    const u32 UTF8_THREE_BYTE_MASK = 0b11100000;
    const u32 UTF8_FOUR_BYTE_MASK  = 0b11110000;
    const u32 UTF8_FOLLOWING_BYTES = 0b00111111;
} // namespace core_internal

// MAX FOR 2 Bytes:
// 0b11000000 - 129
// 0b00011111 - 31
// 0b11011111 - 223

// MAX FOR ENCODED CHUNKS:
// 0b10111111 - 191

static bool8 __IsValidUTF8Encoding(constptr uchar* _utf8Seq, i32 _len) {
  bool8 res = false;
  switch(_len) {
      case 0:
        res = true;
        break;
      case 1:
        res = FirstNBitsAre(_utf8Seq[0], 1, 0) &&
              (u8)_utf8Seq[0] <= MAX_I8;
        break;
      case 2:
        res = FirstNBitsAre(_utf8Seq[0], 3, 0b110) &&
              FirstNBitsAre(_utf8Seq[1], 2, 0b10);
        break;
      case 3:
        res = FirstNBitsAre(_utf8Seq[0], 4, 0b1110) &&
              FirstNBitsAre(_utf8Seq[1], 2, 0b10) &&
              FirstNBitsAre(_utf8Seq[2], 2, 0b10);
        break;
      case 4:
        res = FirstNBitsAre(_utf8Seq[0], 5, 0b11110) &&
              FirstNBitsAre(_utf8Seq[1], 2, 0b10) &&
              FirstNBitsAre(_utf8Seq[2], 2, 0b10) &&
              FirstNBitsAre(_utf8Seq[3], 2, 0b10);
        break;
  }
  return res;
}

Optional<rune> RuneFromUTF8Sequence(constptr uchar* _utf8Seq, i32 _len) {
    Assert(_utf8Seq != null);
    AssertMsg(_len <= 4, "Can't encode more than 4 bytes in a single rune.");

    if (__IsValidUTF8Encoding(_utf8Seq, _len) == false) {
        return Optional<rune>(0, "Invalid UTF-8 encoding.");
    }

    rune r;
    switch(_len) {
        case 1: {
            r = (rune)_utf8Seq[0];
            break;
        }
        case 2: {
            u32 b0 = (u32)_utf8Seq[0] & ~core_internal::UTF8_TWO_BYTE_MASK;
            u32 b1 = (u32)_utf8Seq[1] & core_internal::UTF8_FOLLOWING_BYTES;
            r = (b0 << 6) | (b1);
            break;
        }
        case 3: {
            u32 b0 = (u32)_utf8Seq[0] & ~core_internal::UTF8_THREE_BYTE_MASK;
            u32 b1 = (u32)_utf8Seq[1] & core_internal::UTF8_FOLLOWING_BYTES;
            u32 b2 = (u32)_utf8Seq[2] & core_internal::UTF8_FOLLOWING_BYTES;
            r = (b0 << 12) | (b1 << 6) | (b2);
            break;
        }
        case 4: {
            u32 b0 = (u32)_utf8Seq[0] & ~core_internal::UTF8_FOUR_BYTE_MASK;
            u32 b1 = (u32)_utf8Seq[1] & core_internal::UTF8_FOLLOWING_BYTES;
            u32 b2 = (u32)_utf8Seq[2] & core_internal::UTF8_FOLLOWING_BYTES;
            u32 b3 = (u32)_utf8Seq[3] & core_internal::UTF8_FOLLOWING_BYTES;
            r = (b0 << 18) | (b1 << 12) | (b2 << 6) | b3;
            break;
        }
    }

    return Optional<rune>(r, null);
}

Optional<i32> RuneToUTF8Sequence(rune _rune, modptr uchar* _utf8Seq) {
    Assert(_utf8Seq != null);
    i32 len = 0;

    if (((u32)_rune >> 18) > 1) {
        // 4 symbols
    } else if (((u32)_rune >> 12) > 1) {
        // 3 symbols
    } else if (((u32)_rune >> 6) > 1) {
        // 2 symbols
    } else if ((u32)_rune > 1) {
        // 1 symbols
        // _utf8Seq[0] = ((u32)_rune & (~(u32)__UTF8_BYTE_MASK::ONE_BYTE));
    }

    // uchar v = (uchar)((u32)_rune & (u32)__UTF8_BYTE_MASK::ONE_BYTE << 6);
    // uchar v;
    // v = (uchar)((u32)_rune & ((u32)__UTF8_BYTE_MASK::ONE_BYTE << 18));
    // v = (uchar)((u32)_rune & ((u32)__UTF8_BYTE_MASK::ONE_BYTE << 12));
    // v = (uchar)((u32)_rune & ((u32)__UTF8_BYTE_MASK::ONE_BYTE << 6));
    // v = (uchar)((u32)_rune & ((u32)__UTF8_BYTE_MASK::ONE_BYTE));

    return Optional<i32>(len, null);
}

} // namespace core