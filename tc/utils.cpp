#include <fstream>
#include <cassert>

#include "utils.h"
/// <summary>
/// Pack RGBA components into a single 32-bit integer
/// </summary>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
/// <param name="a"></param>
/// <returns></returns>
uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
    return (a<<24) + (b<<16) + (g<<8) + r;
}
/// <summary>
/// Unpack a 32-bit integer into RGBA components
/// </summary>
/// <param name="color"></param>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
/// <param name="a"></param>
void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
    r = (color >>  0) & 255;
    g = (color >>  8) & 255;
    b = (color >> 16) & 255;
    a = (color >> 24) & 255;
}
/// <summary>
/// Save an image as a PPM file
/// </summary>
/// <param name="filename"></param>
/// <param name="image"></param>
/// <param name="w"></param>
/// <param name="h"></param>
void drop_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const size_t w, const size_t h) {
    assert(image.size() == w*h);
    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P6\n" << w << " " << h << "\n255\n";
    for (size_t i = 0; i < h*w; ++i) {
        uint8_t r, g, b, a;
        unpack_color(image[i], r, g, b, a);
        ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
    }
    ofs.close();
}

