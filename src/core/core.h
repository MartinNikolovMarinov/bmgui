#ifndef CORE_H
#define CORE_H 1

#include "types.h"

namespace core
{

enum class ErrCodes {
    OK               = 0x000,
    ERROR            = 0x001,
    UNEXPECTED_PANIC = 0x255
};

#pragma region Print

i32 PrintF(constptr char* _format, ...);
i32 PrintErrF(constptr char* _format, ...);
i32 SprintF(modptr char* _s, constptr char* _format, ...);

#pragma endregion Print

#pragma region Debug

void inline __assertFailedHandler(constptr char *_file, i32 _line, constptr char *_expr, constptr char *_failMsg)
{
    PrintErrF(" [FILE] %s [LINE] %d [EXPR] %s [MSG] %s\n", _file, _line, _expr, _failMsg);
}

void inline __panicHandler(constptr char *_msg)
{
    PrintErrF(" [PANIC] %s\n", _msg);
}

#if defined(DEBUG) && (DEBUG == 1)
    #define Assert(expr)                                                      \
        if (!(expr)) {                                                        \
            core::__assertFailedHandler(__FILE__, __LINE__, #expr, "");       \
            *(i32 *)0 = 0;                                                    \
        }

    #define AssertMsg(expr, errMsg)                                           \
        if (!(expr)) {                                                        \
            core::__assertFailedHandler(__FILE__, __LINE__, #expr, errMsg);   \
            *(i32 *)0 = 0;                                                    \
        }
#else
    #define Assert(expr)
    #define AssertMsg(expr, errMsg)
#endif

#define Panic(msg)                                      \
    core::__panicHandler(msg);                          \
    core::PltExit((i32)core::ErrCodes::UNEXPECTED_PANIC)

#pragma endregion Debug

#pragma region Try

/**
 * \brief Tryies to extract the value from an Optional. Returns the Optional, if the error is NOT null.
 *        Useful when having to communicate errors in many functions with the same signature.
 *        Using this macro might require a change in the method signature to fit the correct return type.
 *
 * \param expr The expression should evaluate to an Optional value object.
 *
 * \example
 * i32 v = TryOrReturn(RetOptI32());
 * Expands to:
 * i32 v = ({ auto opt = (RetOptI32()); if (opt.err != null) { return opt; } opt.val; })
 */
#define TryOrReturn(expr)                    \
    ({                                       \
        auto opt = (expr);                   \
        if (opt.err != null) { return opt; } \
        opt.val;                             \
    })

/**
 * \brief Hard crashes the program if an Optional value has an error.
 *        When DEBUG=true will assert with the error message.
 *        When DEBUG=false will panic with the error message.
 *
 * \param expr The expression should evaluate to an Optional value object.
 *
 * \example
 * i32 v = TryOrFail(RetOptI32());
 * Expands to:
 * i32 v = ({ auto opt = (RetOptI32()); AssertMsg(opt.err == null, opt.err); opt.val; });
 * or:
 * i32 v = ({ auto opt = (RetOptI32(0)); if (opt.err != null) Panic(opt.err); opt.val; });
 */
#if defined(DEBUG) && (DEBUG == 1)
    #define TryOrFail(expr)                             \
        ({                                              \
            auto opt = (expr);                          \
            AssertMsg(opt.err == null, opt.err);        \
            opt.val;                                    \
        })
#else
    // In DEBUG mode AssertMsg is not set to anything, so panic instead:
    #define TryOrFail(expr)                             \
        ({                                              \
            auto opt = (expr);                          \
            if (opt.err != null) Panic(opt.err);        \
            opt.val;                                    \
        })
#endif

#pragma endregion Try

#pragma region Integers

bool8 IsInRange(u32 _min, u32 _n, u32 _max);
bool8 IsInRangeInc(u32 _min, u32 _n, u32 _max);
bool8 IsInRange(i32 _min, i32 _n, i32 _max);
bool8 IsInRangeInc(i32 _min, i32 _n, i32 _max);

i32 DigitCountI64(i64 _n);
i32 DigitCountI32(i32 _n);

#pragma endregion Integers

#pragma region Math

#define PI32 3.14159265359f

i64 PowI32(i32 _n, i32 _power);
i64 PowI64(i64 _n, i32 _power);
f32 PowF32(f32 _n, i32 _power);
f64 PowF64(f64 _n, i32 _power);

#pragma endregion Math

#pragma region Platform

void PltExit(i32 code);

#pragma endregion Platform

#pragma region Raw String

u64 StrLen(constptr char* _p);
i64 I32ToChar(modptr char* _out, i32 _n);
i64 I64ToChar(modptr char* _out, i64 _n);
i32 StrCmp(constptr char* _a, constptr char* _b);
char LeastSignificatDigitI32ToChar(i32 _digit);
char LeastSignificatDigitI64ToChar(i64 _digit);

#pragma endregion Raw String

#pragma region Rune

Optional<rune> RuneFromUTF8Sequence(constptr uchar* _utf8Seq, i32 _len);
Optional<i32> RuneToUTF8Sequence(rune _rune, modptr uchar* _utf8Seq);

#pragma endregion Rune

#pragma region Random

void InitRand();
i32 RandI32(i32 _min, i32 _max);
void RandRawStr(char* _data, i32 _len);

#pragma endregion Random

#pragma region Time

u64 GetTimeNowMS();
void ThreadSleep(u64 ms);

#pragma endregion Time

#pragma region Bits

bool8 FirstNBitsAre(u8 _v, u8 _n, u8 _bits);

#pragma endregion Bits

} // namespace core

#endif