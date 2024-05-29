#ifndef TEXTURES_H
#define TEXTURES_H

#include <vector>
#include <cstdint>

struct Texture {
    size_t img_w, img_h; 
    size_t count, size; 
    std::vector<uint32_t> img;

    Texture(const std::string filename, const uint32_t format);
    uint32_t get(const size_t i, const size_t j, const size_t idx) const; // get the pixel (i,j) from the texture idx
    std::vector<uint32_t> get_scaled_column(const size_t texture_id, const size_t tex_coord, const size_t column_height) const; // retrieve one column (tex_coord) from the texture texture_id and scale it to the destination size

    void operator=(Texture& T)
    {
        this->img_w = T.img_w;
        this->img_h = T.img_h;
        this->count = T.count;
        this->size = T.size;
        this->img = T.img;
    }
};
/// <summary>
/// // Texture factory for creating Texture objects
/// </summary>
class TextureFactory
{
public: 
    static Texture createTexture(const std::string& filename, uint32_t format) {
        return Texture(filename, format);
    }
};

#endif // TEXTURES_H

