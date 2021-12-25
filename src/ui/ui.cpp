#include "ui.h"

namespace ui
{

void RectScale(modptr Rect *_rect, f32 _scalar)
{
    Assert(_rect != null);
    _rect->x -= (i32)((f32)_rect->w * _scalar / 2);
    _rect->y -= (i32)((f32)_rect->h * _scalar / 2);
    _rect->w += (i32)((f32)_rect->w * _scalar);
    _rect->h += (i32)((f32)_rect->h * _scalar);
}

Rect RectScaleCopy(constptr Rect *_rect, f32 _scalar)
{
    Assert(_rect != null);
    Rect cpy = *_rect;
    RectScale(&cpy, _scalar);
    return cpy;
}

} // namespace ui
