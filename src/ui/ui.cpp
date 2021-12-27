#include "ui.h"

namespace ui
{

void RectScale(modptr Rect *_rect, i32 _scalar)
{
    Assert(_rect != null);
    _rect->x -= (i32)(_scalar);
    _rect->y -= (i32)(_scalar);
    _rect->w += (i32)(_scalar * 2);
    _rect->h += (i32)(_scalar * 2);
}

Rect RectScaleCopy(constptr Rect *_rect, i32 _scalar)
{
    Assert(_rect != null);
    Rect cpy = *_rect;
    RectScale(&cpy, _scalar);
    return cpy;
}

} // namespace ui
