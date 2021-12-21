#ifndef UI_H
#define UI_H 1

#include "../core/core.h"
#include "../core/types.h"

namespace ui
{
    struct Vec2D    { i32 x, y; };
    struct Rect     { i32 x, y, w, h; };
    struct RGBColor { u8 r, g, b, a; };

    enum class Alpha {
        Transperant = 0,
        Opeque      = 255
    };

    // Constant colors:
    global_variable RGBColor Black = { 0x00, 0x00, 0x00, (u8)Alpha::Opeque };
    global_variable RGBColor White = { 0xFF, 0xFF, 0xFF, (u8)Alpha::Opeque };
    global_variable RGBColor Red   = { 0xFF, 0x00, 0x00, (u8)Alpha::Opeque };
    global_variable RGBColor Green = { 0x00, 0xFF, 0x00, (u8)Alpha::Opeque };
    global_variable RGBColor Blue  = { 0x00, 0x00, 0xFF, (u8)Alpha::Opeque };
    global_variable RGBColor Cyan  = { 0x00, 0xFF, 0xFF, (u8)Alpha::Opeque };

    enum class BlendMode {
        /*
            No Blending
                dstRGBA = srcRGBA
        */
        BLENDMODE_NONE = 0x00000000,
        /*
            Alpha Blending
                dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
                dstA = srcA + (dstA * (1-srcA))
        */
        BLENDMODE_BLEND = 0x00000001,
        /*
            Additive Blending
                dstRGB = (srcRGB * srcA) + dstRGB
                dstA = dstA
        */
        BLENDMODE_ADD = 0x00000002,
        /*
            Color Modulate
                dstRGB = srcRGB * dstRGB
                dstA = dstA
        */
        BLENDMODE_MOD = 0x00000004,
        /*
            Color Multiply
                dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA))
                dstA = (srcA * dstA) + (dstA * (1-srcA))
        */
        BLENDMODE_MUL = 0x00000008,
    };

    struct Key {
        bool8 isDown, isHeld;
        i32 keyCode, keyUSBScanCode;
    };

    enum class ModifierFlags {
        NONE        = 0,
        CTRL        = 1 << 0,
        SHIFT       = 1 << 1,
        ALT         = 1 << 2,
        CAPS        = 1 << 3,
        NUM_LOCK    = 1 << 4,
        CAPS_LOCK   = 1 << 5,
        SCROLL_LOCK = 1 << 6,
    };

    const i32 MAX_PRESSED_KEYS = 50; // FIXME: If more than 50 keys are pressed at the same time the program will crash?

    struct Keyboard {
        ModifierFlags modifierFlags;
        Key keys[MAX_PRESSED_KEYS]; // max pressed keys on a single frame = 15
        i32 keysLen;
    };

    enum class MouseBtn {
        LEFT,
        MIDDLE,
        RIGHT,
        DOUBLE,
        MAX
    };

    struct Mouse {
        MouseBtn buttons[(i32)MouseBtn::MAX];
        Vec2D pos, prevPos, delta, scrollDelta;
        bool8 isGrabbing, isGrabbed, isUnGrabbed;
    };

    struct Input {
        Keyboard keyboard;
        Mouse mouse;
    };

    struct UiCtx {
        Input input;
    };
} // namespace ui

#endif