#ifndef CORE_TYPES_H
#define CORE_TYPES_H 1

// This include allowes the use of variadic functions.
// It is a 0 cost abstraction that defines a couple of macros.
#include <stdarg.h>

// Include system defaults.
#include <stdint.h>
#include <stddef.h>

/*
    IMPORTANT:
    It's best to avoid adding any includes to this file to avoid cyclic dependencies,
    because it is literarily included in every other file.
    Also, this files is not in the core namespace on purpose.
*/

#define global_variable static  // Defined Global Variable marker macro
#define constptr const          // Constant Pointer marker macro
#define modptr                 // Modifiable Pointer marker macro

template<typename T>
struct Optional {
    T val;
    constptr char* err;

    Optional() {}
    Optional(T _val, constptr char* _err) : val(_val), err(_err) {}
};

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define MAX_U64 ((u64)0xFFFFFFFFFFFFFFFF)             // 18446744073709551615
#define MAX_U32 ((u32)(MAX_U64 >> (sizeof(u32) * 8))) // 4294967295
#define MAX_U16 ((u16)(MAX_U32 >> (sizeof(u16) * 8))) // 65535
#define MAX_U8  ((u8)(MAX_U16 >> (sizeof(u8) * 8)))   // 255

#define MAX_I64 ((i64)(MAX_U64 >> 1))                 // 9223372036854775807
#define MAX_I32 ((i32)(MAX_I64 >> (sizeof(i32) * 8))) // 2147483647
#define MAX_I16 ((i16)(MAX_I32 >> (sizeof(i16) * 8))) // 32767
#define MAX_I8  ((i8)(MAX_I16 >> (sizeof(i8) * 8)))   // 127

#define MIN_I64 ((i64)((MAX_U64 >> 1) ^ MAX_U64))     // -9223372036854775808
#define MIN_I32 ((i32)((MAX_U32 >> 1) ^ MAX_U32))     // -2147483648
#define MIN_I16 ((i16)((MAX_U16 >> 1) ^ MAX_U16))     // -32768
#define MIN_I8  ((i8)((MAX_U8 >> 1) ^ MAX_U8))        // -128

typedef u8 byte;

typedef float  f32;
typedef double f64;

typedef u8  bool8;
typedef u32 bool32;

typedef unsigned char uchar;

// Runes represent a single UTF-32 encoded character.
typedef char32_t rune;

// Pointer size specific types:
typedef size_t index_size;

// Special constants :
#define true  (1)
#define false (0)
#define null  (0)
#define term_char ('\0')

#define STDIN  (0)
#define STDOUT (1)
#define STDERR (2)

// Storage Sizes
#define BYTE        ((i64)1)
#define KILOBYTE    ((i64)(1000 * BYTE))
#define MEGABYTE    ((i64)(1000 * KILOBYTE))
#define GIGABYTE    ((i64)(1000 * MEGABYTE))
#define TERABYTE    ((i64)(1000 * GIGABYTE))

// Duration Types
#define NANOSECOND  ((u64)1)
#define MICROSECOND ((u64)(1000 * NANOSECOND))  //             1_000ns
#define MILLISECOND ((u64)(1000 * MICROSECOND)) //         1_000_000ns
#define SECOND      ((u64)(1000 * MILLISECOND)) //     1_000_000_000ns
#define MINUTE      ((u64)(60 * SECOND))        //    60_000_000_000ns
#define HOUR        ((u64)(60 * MINUTE))        // 3_600_000_000_000ns

#endif /* TYPES_H */
