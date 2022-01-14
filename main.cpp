#include "./src/unity_build.h"

#include <SDL2/SDL.h>

using namespace core;
using namespace ui;

/**
 * \brief Loads and image from a file.
 *
 * \param _img The image carrying the pixel data will be set.
 * \param _path The path to the image on the file system.
 *
 * \returns Optional with error code.
*/
Optional<i32> ImageFromFile(Image *_img, constptr char* _path)
{
    Assert(_img != null);
    Assert(_path != null);
    SDL_Surface* data = SDL_LoadBMP(_path);
    if (data == null) {
        return Optional<i32>((i32)core::ErrCodes::ERROR, SDL_GetError());
    }
    _img->pixelData = (void*)data;
    _img->w = data->w;
    _img->h = data->h;
    return Optional<i32>((i32)core::ErrCodes::OK, null);
}

/**
 * \brief Converts pixel data from an image to a driver specifc texture.
 *
 * \param _rendere The driver specifc renderer.
 * \param _img The image carrying the pixel data that will be copied to the texture.
 * \param _out The output texture.
 *
 * \returns Optional with error code.
*/
Optional<i32> ImageToTexture(SDL_Renderer *_renderer, constptr Image* _img, modptr SDL_Texture** _out)
{
    Assert(_renderer != null);
    Assert(_img != null);
    Assert(_out != null);
    SDL_Surface *sdlSurface = (SDL_Surface *)_img->pixelData;
    Assert(sdlSurface != null);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, sdlSurface);
    if (texture == null) {
        return Optional<i32>((i32)core::ErrCodes::ERROR, SDL_GetError());
    }
    *_out = texture;
    return Optional<i32>((i32)core::ErrCodes::OK, null);
}

/**
 * \brief The color key defines a pixel value that will be treated as transparent.
 *        In other words, the pixel will be ignored when rendering.
 *        This is similar to a visual-effects and post-production technique called
 *        chroma keys https://en.wikipedia.org/wiki/Chroma_key
 *
 * \param _img The image carrying the pixel data.
 * \param _color The red, green and blue values of the color key pixel.
 *
 * \returns Optional with error code.
*/
Optional<i32> ImageSetColorKey(modptr Image *_img, constptr RGBColor *_color)
{
    Assert(_img != null);
    Assert(_color != null);
    SDL_Surface *sdlSurface = (SDL_Surface *)_img->pixelData;
    u32 pixel = SDL_MapRGB(sdlSurface->format, 0, 0xFF, 0xFF);
    i32 errCode = SDL_SetColorKey(sdlSurface, SDL_TRUE, pixel);
    if (errCode != 0) {
        return Optional<i32>(errCode, SDL_GetError());
    }
    return Optional<i32>(errCode, null);
}

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

SDL_Rect ToSDLRect(constptr Rect *_r)
{
    Assert(_r != null);
    SDL_Rect ret = (SDL_Rect){ _r->x, _r->y, _r->w, _r->h };
    return ret;
}

char* DEBUG_ModifierToCharPtr(ModifierFlags mod, modptr char* buff)
{
    // TODO: implement strcat!
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
    if (keyCode == SDLK_LCTRL || keyCode == SDLK_RCTRL)     return ModifierFlags::CTRL;
    if (keyCode == SDLK_LSHIFT || keyCode == SDLK_RSHIFT)   return ModifierFlags::SHIFT;
    if (keyCode == SDLK_LALT || keyCode == SDLK_RALT)       return ModifierFlags::ALT;
    if (keyCode == SDLK_CAPSLOCK)                           return ModifierFlags::CAPS_LOCK;
    if (keyCode == SDLK_NUMLOCKCLEAR)                       return ModifierFlags::NUM_LOCK;
    if (keyCode == SDLK_SCROLLLOCK)                         return ModifierFlags::SCROLL_LOCK;

    return ModifierFlags::NONE;
}

void HandleKeyPress(constptr SDL_Event *_event, modptr UiCtx* _ctx)
{
    i32 keyCode = _event->key.keysym.sym;
    i32 scanCodeFromUSB = _event->key.keysym.scancode;
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

    // Iterate over the previously set keyboard keys and start setting currently pressed keys.
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
        Key key = { true, false, keyCode, scanCodeFromUSB };
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
                key.keyCode, key.isDown, key.isHeld, key.KeyScanCodeUSB, buf);
    }
}

void HandleEvent(constptr SDL_Event *_event, modptr bool8 *_quit, modptr UiCtx* _ctx)
{
    if (_event->type == SDL_QUIT) {
        *_quit = true;
        return;
    }

    bool8 isKeyPress = (_event->type == SDL_KEYDOWN || _event->type == SDL_KEYUP);
    if (isKeyPress == true) {
        HandleKeyPress(_event, _ctx);
        return;
    } else if (_event->type == SDL_TEXTINPUT) {
        // TODO: Debug code:
        i32 textLen = CharPtrLen(_event->text.text);
        Optional<rune> optRune = RuneFromUTF8Sequence((uchar*)_event->text.text, textLen);
        if (optRune.err != null) {
            char panicMsg[50];
            SprintF(panicMsg, "Failed to parse UTF-32 rune in %s \n", _event->text.text);
            Panic(panicMsg);
        }
        char testText[SDL_TEXTINPUTEVENT_TEXT_SIZE] = {};
        RuneToUTF8Sequence(optRune.val, (uchar*)testText);
        AssertMsg(CharPtrCmp(testText, _event->text.text) == 0, "UTF-8 Encode/Decode is failing");

        PrintF("EVENT TEXT INPUT: value = %s, UTF-32 rune = %lu \n", _event->text.text, optRune.val);
        return;
    } else if (_event->type == SDL_KEYMAPCHANGED) {
        // TODO: Keymapchange seems to happen on every button press. Why ?
        return;
    } else if (_event->type == SDL_MOUSEMOTION) {
        _ctx->input.mouse.prevPos.x = _ctx->input.mouse.pos.x;
        _ctx->input.mouse.prevPos.y = _ctx->input.mouse.pos.y;
        _ctx->input.mouse.pos.x = _event->motion.x;
        _ctx->input.mouse.pos.y = _event->motion.y;
        _ctx->input.mouse.delta.x = _event->motion.xrel;
        _ctx->input.mouse.delta.y = _event->motion.yrel;

        // // TODO: Debug code:
        // PrintF("EVENT MOUSE MOVE: x = %d, y = %d, dx = %d, dy = %d, px = %d, py = %d\n",
        //         _event->motion.x, _event->motion.y,
        //         _ctx->input.mouse.delta.x, _ctx->input.mouse.delta.y,
        //         _ctx->input.mouse.prevPos.x, _ctx->input.mouse.prevPos.y);
        return;
    } else if (_event->type == SDL_MOUSEWHEEL) {
        _ctx->input.mouse.scrollDelta.x = _event->wheel.x;
        _ctx->input.mouse.scrollDelta.y = _event->wheel.y;
        return;
    } else if (_event->type == SDL_MOUSEBUTTONDOWN || _event->type == SDL_MOUSEBUTTONUP) {
        // PrintF("", _event->button.button);
        return;
    } else if (_event->type == SDL_WINDOWEVENT) {
        // System specific window events.
        return;
    }

    // TODO: Debug code:
    PrintF("!UNHANDLED EVENT TRIGGERED RERENDER (type: %d)!\n", _event->type);
}

Optional<i32> DebugRenderRect(SDL_Renderer *_renderer, Rect _rect, RGBColor _color)
{
    Assert(_renderer != null);

    i32 errCode;
    if (errCode = SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a); errCode != 0) {
        return Optional<i32>(errCode, SDL_GetError());
    }
    SDL_Rect sdlRect = { _rect.x, _rect.y, _rect.w, _rect.h };
    if (errCode = SDL_RenderFillRect(_renderer, &sdlRect); errCode != 0) {
        return Optional<i32>(errCode, SDL_GetError());
    }

    return Optional<i32>(errCode, null);
}

Optional<i32> RenderUiRectComponent(SDL_Renderer *_renderer, UiComp *_comp)
{
    Assert(_renderer != null);
    Assert(_comp != null);

    if (_comp->boarder.top > 0) {
        Rect topBoarderRect;
        topBoarderRect.x = _comp->rect.x - _comp->boarder.left; // Compensate for left boarder.
        topBoarderRect.y = _comp->rect.y - _comp->boarder.top;
        topBoarderRect.w = _comp->rect.w + _comp->boarder.left; // Compensate for left boarder.
        topBoarderRect.h = _comp->boarder.top;
        TryOrReturn(DebugRenderRect(_renderer, topBoarderRect, _comp->boarderColorTop));
    }
    if (_comp->boarder.right > 0) {
        Rect leftBoarder;
        leftBoarder.x = _comp->rect.x + _comp->rect.w;
        leftBoarder.y = _comp->rect.y - _comp->boarder.top; // Compensate for top boarder.
        leftBoarder.w = _comp->boarder.right;
        leftBoarder.h = _comp->rect.h + _comp->boarder.top; // Compensate for top boarder.
        TryOrReturn(DebugRenderRect(_renderer, leftBoarder, _comp->boarderColorRight));
    }
    if (_comp->boarder.bottom > 0) {
        Rect bottomBoarderRect;
        bottomBoarderRect.x = _comp->rect.x;
        bottomBoarderRect.y = _comp->rect.y + _comp->rect.h;
        bottomBoarderRect.w = _comp->rect.w + _comp->boarder.right; // Compensate for right boarder.
        bottomBoarderRect.h = _comp->boarder.bottom;
        TryOrReturn(DebugRenderRect(_renderer, bottomBoarderRect, _comp->boarderColorBottom));
    }
    if (_comp->boarder.left > 0) {
        Rect leftBoarder;
        leftBoarder.x = (_comp->rect.x - _comp->boarder.left);
        leftBoarder.y = _comp->rect.y;
        leftBoarder.w = _comp->boarder.left;
        leftBoarder.h = _comp->rect.h + _comp->boarder.bottom; // Compensate for bottom boarder.
        TryOrReturn(DebugRenderRect(_renderer, leftBoarder, _comp->boarderColorLeft));
    }

    TryOrReturn(DebugRenderRect(_renderer, _comp->rect, _comp->bgColor));

    return Optional<i32>((i32)core::ErrCodes::OK, null);
}


/* SDL Renderer API

 SDL_Texture* SDL_CreateTexture(SDL_Renderer * renderer, Uint32 format, int access, int w, int h);

 int SDL_QueryTexture(SDL_Texture * texture, Uint32 * format, int *access, int *w, int *h);
 int SDL_QueryTexture(SDL_Texture * texture, Uint32 * format, int *access, int *w, int *h);

 int SDL_GetTextureColorMod(SDL_Texture * texture, Uint8 * r, Uint8 * g, Uint8 * b);
 int SDL_SetTextureColorMod(SDL_Texture * texture, Uint8 r, Uint8 g, Uint8 b);

 int SDL_GetTextureAlphaMod(SDL_Texture * texture, Uint8 * alpha);
 int SDL_SetTextureAlphaMod(SDL_Texture * texture, Uint8 alpha);

 int SDL_GetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode *blendMode);
 int SDL_SetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode blendMode);

 int SDL_GetTextureScaleMode(SDL_Texture * texture, SDL_ScaleMode *scaleMode);
 int SDL_SetTextureScaleMode(SDL_Texture * texture, SDL_ScaleMode scaleMode);

 int SDL_UpdateTexture(SDL_Texture * texture, const SDL_Rect * rect, const void *pixels, int pitch);

 int SDL_LockTexture(SDL_Texture * texture, const SDL_Rect * rect, void **pixels, int *pitch);
 void SDL_UnlockTexture(SDL_Texture * texture);

 SDL_bool SDL_RenderTargetSupported(SDL_Renderer *renderer);
 int SDL_SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture);
 SDL_Texture * SDL_GetRenderTarget(SDL_Renderer *renderer);

 int SDL_RenderSetViewport(SDL_Renderer * renderer, const SDL_Rect * rect);
 void SDL_RenderGetViewport(SDL_Renderer * renderer, SDL_Rect * rect);

 int SDL_RenderSetClipRect(SDL_Renderer * renderer, const SDL_Rect * rect);
 void SDL_RenderGetClipRect(SDL_Renderer * renderer, SDL_Rect * rect);
 SDL_bool SDL_RenderIsClipEnabled(SDL_Renderer * renderer);

 int SDL_RenderSetScale(SDL_Renderer * renderer, float scaleX, float scaleY);
 void SDL_RenderGetScale(SDL_Renderer * renderer, float *scaleX, float *scaleY);

 int SDL_SetRenderDrawColor(SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
 int SDL_GetRenderDrawColor(SDL_Renderer * renderer, Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a);

 int SDL_SetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode);
 int SDLCALL SDL_GetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode *blendMode);

 int SDLCALL SDL_RenderClear(SDL_Renderer * renderer);

 int SDL_RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * srcrect, const SDL_Rect * dstrect);
 int SDL_RenderCopyEx(SDL_Renderer * renderer,
                     SDL_Texture * texture,
                     const SDL_Rect * srcrect,
                     const SDL_Rect * dstrect,
                     const double angle,
                     const SDL_Point *center,
                     const SDL_RendererFlip flip);

  int SDL_RenderGeometry(SDL_Renderer *renderer,
                         SDL_Texture *texture,
                         const SDL_Vertex *vertices,
                         int num_vertices,
                         const int *indices,
                         int num_indices);
  int SDL_RenderGeometryRaw(SDL_Renderer *renderer,
                            SDL_Texture *texture,
                            const float *xy, int xy_stride,
                            const SDL_Color *color, int color_stride,
                            const float *uv, int uv_stride,
                            int num_vertices,
                            const void *indices, int num_indices, int size_indices);


*/

i32 main(i32 argc, constptr char **argv, constptr char **_envp)
{
    const i32 DELAY = 40;
    const i32 MAX_EVENTS_PER_FRAME = 20;
    i32 errCode;

    errCode = SDL_Init(SDL_INIT_VIDEO);
    Assert(errCode == 0);
    Assert(SDL_SetHint(SDL_HINT_VIDEO_ALLOW_SCREENSAVER, "1") == SDL_TRUE);
    SDL_EnableScreenSaver();

    SDL_Window *sdlWindow = SDL_CreateWindow("Main Window",
                                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            800, 600, SDL_WINDOW_RESIZABLE);
    Assert(sdlWindow != null);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    Assert(sdlRenderer != null);

    errCode = SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
    Assert(errCode == 0);

    // Image img = {};
    // TryOrFail(ImageFromFile(&img, "./data/imgs/sample_1280×853.bmp"));
    // SDL_Texture *imgTexture;
    // TryOrFail(ImageToTexture(sdlRenderer, &img, &imgTexture));

    UiCtx uiCtx = {};
    bool8 quit = false;
    while (!quit) {
        // Clean screen.
        RGBColor *clearColor = &White;
        errCode = SDL_SetRenderDrawColor(sdlRenderer, clearColor->r, clearColor->g, clearColor->b, clearColor->a);
        Assert(errCode == 0);
        errCode = SDL_RenderClear(sdlRenderer); // NOTE: Ignores viewports and clears the entire screen !
        Assert(errCode == 0);

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

        // IMPORTANT: every element should be it's own texture ??
        UiComp comp1 = {};
        comp1.id = 1;
        comp1.rect = (Rect){ 280, 123, 157, 337 };
        comp1.bgColor = { Red.r, Red.g, Red.b, 150 };
        comp1.boarderColorTop = Black;
        comp1.boarderColorLeft = Green;
        comp1.boarderColorBottom = Blue;
        comp1.boarderColorRight = Cyan;
        comp1.boarder = (Thinkness){ 3, 5, 7, 8 };
        comp1.margin = {};
        comp1.padding = {};

        TryOrFail(RenderUiRectComponent(sdlRenderer, &comp1));

        // SDL_Vertex vertices[3] = {
        //     { (SDL_FPoint){ 400, 200 }, (SDL_Color){ Blue.r, Blue.g, Blue.b, 60 } },
        //     { (SDL_FPoint){ 700, 450 }, (SDL_Color){ Green.r, Green.g, Green.b, 60 } },
        //     { (SDL_FPoint){ 600, 450 }, (SDL_Color){ Blue.r, Blue.g, Blue.b, 60 } }
        // };
        // errCode = SDL_RenderGeometry(sdlRenderer, null, vertices, 3, null, 0);
        // Assert(errCode == 0);

        // Handle events.
        for (i32 i = 0; i < eventsCount; i++) {
            HandleEvent(&events[i], &quit, &uiCtx);
        }

        // Present the renderer backbuffer to screen:
        SDL_RenderPresent(sdlRenderer);
    }

    // SDL_DestroyTexture(imgTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();

    return 0;
}
