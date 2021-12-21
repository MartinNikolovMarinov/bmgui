#include "./src/unity_build.h"

#include <SDL2/SDL.h>

using namespace core;
using namespace ui;

Optional<i32> PullEventFromSDLQueueBlocking(modptr SDL_Event *_event)
{
    i32 errCode = SDL_WaitEvent(_event);
    if (errCode == 0) {
        return Optional<i32>(errCode, SDL_GetError());
    }
    return Optional<i32>(errCode, null);
}

bool8 PullEventFromSDLQueue(modptr SDL_Event *_event)
{
    return SDL_PollEvent(_event) > 0;
}

char* DEBUG_ModifierToCharPtr(ModifierFlags mod, modptr char* buff)
{
    char* curr = buff;
    if(mod == ModifierFlags::NONE) {
        curr = strcat(curr, "NONE");
        return buff;
    }

    if((i32)mod & (i32)ModifierFlags::NUM_LOCK) curr = strcat(curr, "NUMLOCK ");
    if((i32)mod & (i32)ModifierFlags::CAPS_LOCK) curr = strcat(curr, "CAPSLOCK ");
    if((i32)mod & (i32)ModifierFlags::CTRL) curr = strcat(curr, "CTRL ");
    if((i32)mod & (i32)ModifierFlags::SHIFT) curr = strcat(curr, "SHIFT ");
    if((i32)mod & (i32)ModifierFlags::ALT) curr = strcat(curr, "ALT ");

    return buff;
}

bool8 IsNormalModifierKey(i32 keyCode)
{
    bool8 ret = (keyCode == SDLK_LCTRL || keyCode == SDLK_RCTRL ||
                 keyCode == SDLK_LSHIFT || keyCode == SDLK_RSHIFT ||
                 keyCode == SDLK_LALT || keyCode == SDLK_RALT);
    return ret;
}

bool8 IsLocksModifierKey(i32 keyCode)
{
    bool8 ret = (keyCode == SDLK_CAPSLOCK ||
                 keyCode == SDLK_NUMLOCKCLEAR ||
                 keyCode == SDLK_SCROLLLOCK);
    return ret;
}

ModifierFlags SDLModifierToBMGuiModifier(i32 keyCode)
{
    if (keyCode == SDLK_LCTRL || keyCode == SDLK_RCTRL) return ModifierFlags::CTRL;
    if (keyCode == SDLK_LSHIFT || keyCode == SDLK_RSHIFT) return ModifierFlags::SHIFT;
    if (keyCode == SDLK_LALT || keyCode == SDLK_RALT) return ModifierFlags::ALT;
    if (keyCode == SDLK_CAPSLOCK) return ModifierFlags::CAPS_LOCK;
    if (keyCode == SDLK_NUMLOCKCLEAR) return ModifierFlags::NUM_LOCK;
    if (keyCode == SDLK_SCROLLLOCK) return ModifierFlags::SCROLL_LOCK;
    return ModifierFlags::NONE;
}

void HandleKeyPress(constptr SDL_Event *_event, modptr UiCtx* _ctx)
{
    i32 keyCode = _event->key.keysym.sym;
    i32 scanCode = _event->key.keysym.scancode;
    bool8 keyIsDown = (_event->type == SDL_KEYDOWN);

    if (IsNormalModifierKey(keyCode) == true) {
        // Normal key modifiers are added and removed to the modifierFlags on keydown and keyup respectively.
        ModifierFlags modifier = SDLModifierToBMGuiModifier(keyCode);
        Assert(modifier != ModifierFlags::NONE);
        _ctx->input.keyboard.modifierFlags = (ModifierFlags)((i32)_ctx->input.keyboard.modifierFlags ^ ((i32)modifier));
        return;
    }

    if (keyIsDown == true && IsLocksModifierKey(keyCode) == true) {
        // Lock modifiers don't need to be held, instead they are toggled on keydown.
        ModifierFlags modifier = SDLModifierToBMGuiModifier(keyCode);
        Assert(modifier != ModifierFlags::NONE);
        _ctx->input.keyboard.modifierFlags = (ModifierFlags)((i32)_ctx->input.keyboard.modifierFlags ^ ((i32)modifier));
        return;
    }

    if (keyIsDown == false && _ctx->input.keyboard.pressedKeysLen == 0) {
        // This happens when releasing all previously pressed buttons at the same time.
        // In this case, only one keyup event is needed to detect that all keys have been released.
        return;
    }

    bool8 found = false;
    Keyboard oldKeyboardCopy = _ctx->input.keyboard; // Copy the keyboard structure
    _ctx->input.keyboard.pressedKeysLen = 0; // Set the length of the current keyboard keys to 0

    // Iterate over the previously set keyboard keys.
    // Skip over all keys that are NO longer pressed.
    // If the key was found consider it pressed. It's "probably" already considered pressed, but still...
    // Any key that was previously pressed is now considered "held".
    for (i32 i = 0; i < oldKeyboardCopy.pressedKeysLen; i++) {
        Key currKey = oldKeyboardCopy.pressedKeys[i];
        if (keyIsDown) {
            if (currKey.keyCode == keyCode) {
                AssertMsg(found == false, "Should never find a keyCode twice in the key array!");
                found = true;
                if (currKey.isDown == true) {
                    currKey.isHeld = true;
                } else {
                    currKey.isDown = true;
                }
            }
            _ctx->input.keyboard.pressedKeys[_ctx->input.keyboard.pressedKeysLen] = currKey;
            _ctx->input.keyboard.pressedKeysLen++;
        }
    }

    if (keyIsDown == true && found == false) {
        // If the current key press event is keydown, and the key is not found in the keyboard state,
        // it needs to be added to the ui context.
        Key key = { true, false, keyCode, scanCode };
        _ctx->input.keyboard.pressedKeys[_ctx->input.keyboard.pressedKeysLen] = key;
        _ctx->input.keyboard.pressedKeysLen++;
    }

    // Debug print keyboard.
    // TODO: remove debug code:
    char buf[255] = {};
    DEBUG_ModifierToCharPtr(_ctx->input.keyboard.modifierFlags, buf);
    PrintF("EVENT KEY PRESS: keys pressed = %d\n", _ctx->input.keyboard.pressedKeysLen);
    for (i32 i = 0; i < _ctx->input.keyboard.pressedKeysLen; i++) {
        Key key = _ctx->input.keyboard.pressedKeys[i];
        PrintF("EVENT KEY PRESS: keyCode = %d, isDown = %d, isHeld = %d, keyUSBScanCode = %d, modifiers = %s \n",
                key.keyCode, key.isDown, key.isHeld, key.keyUSBScanCode, buf);
    }
}

void HandleEvent(constptr SDL_Event *_event, modptr bool8 *_quit, modptr UiCtx* _ctx)
{
    if (_event->type == SDL_QUIT) {
        *_quit = true;
        return;
    }

    //!! TODO2: Events to read about and handle next: SDL_TEXTINPUT, SDL_KEYMAPCHANGED, SDL_TEXTEDITING !!

    bool8 isKeyPress = (_event->type == SDL_KEYDOWN || _event->type == SDL_KEYUP);
    if (isKeyPress == true) {
        HandleKeyPress(_event, _ctx);
        return;
    } else if (_event->type == SDL_TEXTINPUT) {
        // _event->text.text is created for wide characters.
        i32 textLen = StrLen(_event->text.text);
        Optional<rune> optRune = RuneFromUTF8Sequence((uchar*)_event->text.text, textLen);
        if (optRune.err != null) {
            char panicMsg[50];
            SprintF(panicMsg, "Failed to parse UTF-32 rune in %s \n", _event->text.text);
            Panic(panicMsg);
        }
        PrintF("EVENT TEXT INPUT: value = %s, UTF-32 rune = %lu \n", _event->text.text, optRune.val);
        return;
    } else if (_event->type == SDL_KEYMAPCHANGED) {
        // TODO: Keymapchange seems to happen on every button press. Why ?
        return;
    } else if (_event->type == SDL_MOUSEMOTION ||
               _event->type == SDL_MOUSEBUTTONDOWN ||
               _event->type == SDL_MOUSEBUTTONUP ||
               _event->type == SDL_MOUSEWHEEL
    ) {
        // Mouse state change
        return;
    } else if (_event->type == SDL_WINDOWEVENT) {
        // System specific window events.
        return;
    }

    // TODO: remove debug code:
    PrintF("!UNHANDLED EVENT TRIGGERED RERENDER (type: %d)!\n", _event->type);
}

i32 main(i32 argc, constptr char **argv, constptr char **_envp)
{
    const i32 DELAY = 40;
    const i32 MAX_EVENTS_PER_FRAME = 20;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1");

    SDL_Window *sdlWindow = SDL_CreateWindow("Main Window",
                                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);

    UiCtx uiCtx = {};
    bool8 quit = false;
    while (!quit) {
        // Clean screen.
        RGBColor *clearColor = &White;
        SDL_SetRenderDrawColor(sdlRenderer, clearColor->r, clearColor->g, clearColor->b, clearColor->a);
        SDL_RenderClear(sdlRenderer);

        // Block until first event. This prevents useless work while window is inactive.
        SDL_Event events[MAX_EVENTS_PER_FRAME];
        i32 eventsCount = 1;
        TryOrFail(PullEventFromSDLQueueBlocking(&events[0]));

        if (DELAY > 0 &&
            (events[0].type == SDL_KEYDOWN || events[0].type == SDL_KEYUP)
        ) {
            /*
                IMPORTANT:
                Some user interactions require a more fine tuned approach to minimize the chance of missing an
                interaction. The tradeoff is a slight rendering slowdown, which duration is controlled by DELAY.
                After the first event comes, poll for every pending event during the last DELAY ms.
            */
            SDL_Delay(DELAY);
            for (i32 i = 1; i < MAX_EVENTS_PER_FRAME; i++) {
                if (PullEventFromSDLQueue(&events[i]) == false) {
                    break;
                }
                eventsCount++;
            }
        }

        // Handle events.
        for (i32 i = 0; i < eventsCount; i++) {
            HandleEvent(&events[i], &quit, &uiCtx);
        }

        // Present the renderer backbuffer to screen:
        SDL_RenderPresent(sdlRenderer);
    }

    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();

    return 0;
}
