#ifndef UI_H
#define UI_H 1

#include "../../lib/core/src/core.h"
#include "../../lib/core/src/types.h"

namespace ui
{
    struct Rect { i32 x, y, w, h; };
    void RectScale(modptr Rect *_rect, i32 _scalar);
    Rect RectScaleCopy(constptr Rect *_rect, i32 _scalar);

    struct Vec2D    { i32 x, y; };

    struct RGBColor { u8 r, g, b, a; };

    struct Thinkness { i32 left, top, right, bottom; };

    enum struct Alpha : u8 {
        Transperant = 0,
        Opeque      = 255
    };

    // Constant colors:
    global_variable RGBColor NoColor = { 0x00, 0x00, 0x00, (u8)Alpha::Transperant };
    global_variable RGBColor Black   = { 0x00, 0x00, 0x00, (u8)Alpha::Opeque };
    global_variable RGBColor White   = { 0xFF, 0xFF, 0xFF, (u8)Alpha::Opeque };
    global_variable RGBColor Red     = { 0xFF, 0x00, 0x00, (u8)Alpha::Opeque };
    global_variable RGBColor Green   = { 0x00, 0xFF, 0x00, (u8)Alpha::Opeque };
    global_variable RGBColor Blue    = { 0x00, 0x00, 0xFF, (u8)Alpha::Opeque };
    global_variable RGBColor Cyan    = { 0x00, 0xFF, 0xFF, (u8)Alpha::Opeque };

    struct Key {
        bool8 isDown, isHeld;
        i32 keyCode, KeyScanCodeUSB;
    };

    enum struct ModifierFlags : u32 {
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
        Key pressedKeys[MAX_PRESSED_KEYS]; // max pressed keys on a single frame = 15
        i32 pressedKeysLen;
    };

    enum struct MouseBtn : u32 {
        LEFT,
        MIDDLE,
        RIGHT,
        DOUBLE,
        MAX
    };

    struct Mouse {
        MouseBtn buttons[(i32)MouseBtn::MAX];
        Vec2D pos, prevPos;
        /**
         * The differential of prev x and y and the current x and y.
        */
        Vec2D delta;
        /**
         * The mouse scroll delta is a Vec2D on propose, because some devices have y-axis scroll wheel. Might not
         * want to support this.
        */
        Vec2D scrollDelta;
        // bool8 isGrabbing, isGrabbed, isUnGrabbed; // TODO: implement mouse drag.
    };

    struct Input {
        Keyboard keyboard;
        Mouse mouse;
    };

    /**
     * UI Context is where the entire state for the library is stored.
    */
    struct UiCtx {
        Input input = {};

        // MemoryAllocator *allocator; // TODO: think about memory allocation interface.
    };

    struct Image {
        i32 w, h;
        void *pixelData;
    };

    /**
     * UI Component.
    */
    struct UiComp {
        i32 id = 0;
        Rect rect = {};
        Thinkness margin = {};
        Thinkness padding = {};

        // f32 boarderRadius; // TODO: use bezier curves for rounded corners ?

        RGBColor bgColor = NoColor;
        RGBColor boarderColorTop = NoColor;
        RGBColor boarderColorLeft = NoColor;
        RGBColor boarderColorBottom = NoColor;
        RGBColor boarderColorRight = NoColor;

        /**
         * Boarder sizes are in pixels.
        */
        Thinkness boarder = {};

        // TextureStyle *bgTextureStyle; // TODO: think about texture background.
        // TextureStyle *boarderTextureStyle;

        // TODO: Think about inherited styles and how styles will be grouped in general.
    };
} // namespace ui

#endif