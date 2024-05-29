#include <cassert>

#include "framebuffer.h"
#include "utils.h"
/// <summary>
/// set_pixel
/// </summary>
/// <param name="x">X cords of pixel</param>
/// <param name="y"></param>
/// <param name="color"></param>

void FrameBuffer::set_pixel(const size_t x, const size_t y, const uint32_t color) 
{
    assert(img.size()==w*h && x<w && y<h);
    img[x+y*w] = color;
}
/// <summary>
/// draw_rectangle
/// </summary>
/// <param name="rect_x"></param>
/// <param name="rect_y"></param>
/// <param name="rect_w"></param>
/// <param name="rect_h"></param>
/// <param name="color"></param>
void FrameBuffer::draw_rectangle(const size_t rect_x, const size_t rect_y, const size_t rect_w, const size_t rect_h, const uint32_t color)///TestAfterHat
{
    assert(img.size()==w*h);
    for (size_t i=0; i<rect_w; i++) {
        for (size_t j=0; j<rect_h; j++) {
            size_t cx = rect_x+i;
            size_t cy = rect_y+j;
            if (cx<w && cy<h) // no need to check for negative values (unsigned variables)
                set_pixel(cx, cy, color);
        }
    }
}

void FrameBuffer::clear(const uint32_t color) {
    img = std::vector<uint32_t>(w*h, color);
}

