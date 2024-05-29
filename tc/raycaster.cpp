#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <cassert>
#include "utils.h"
#include "raycaster.h"
#include <SDL.h>
Uint32 startTime = SDL_GetTicks();
Uint32 elapsedTime = SDL_GetTicks() - startTime;

/// <summary>
/// // Calculate wall texture coordinate based on hit position
/// </summary>
/// <param name="hitx"></param>
/// <param name="hity"></param>
/// <param name="tex_walls"></param>
/// <returns></returns>
int wall_x_texcoord(const float hitx, const float hity, const Texture &tex_walls) {
    float x = hitx - floor(hitx+.5);
    float y = hity - floor(hity+.5);
    int tex = x*tex_walls.size;
    if (std::abs(y)>std::abs(x)) 
        tex = y*tex_walls.size;
    if (tex<0) 
        tex += tex_walls.size;
    assert(tex>=0 && tex<(int)tex_walls.size);
    return tex;
}
/// <summary>
/// // Draw the map and sprites onto the framebuffer
/// </summary>
/// <param name="fb"></param>
/// <param name="sprites"></param>
/// <param name="tex_walls"></param>
/// <param name="map"></param>
/// <param name="cell_w"></param>
/// <param name="cell_h"></param>
void draw_map(FrameBuffer &fb, const std::vector<Sprite> &sprites, const Texture &tex_walls, const Map &map, const size_t cell_w, const size_t cell_h)
{
    for (size_t j=0; j<map.h; j++) {  // draw the map itself
        for (size_t i=0; i<map.w; i++) {
            if (map.is_empty(i, j)) continue; // skip empty spaces
            size_t rect_x = i*cell_w;
            size_t rect_y = j*cell_h;
            size_t texid = map.get(i, j);
            assert(texid<tex_walls.count);
            fb.draw_rectangle(rect_x, rect_y, cell_w, cell_h, tex_walls.get(0, 0, texid)); // the color is taken from the upper left pixel of the texture #texid
        }
    }
    for (size_t i=0; i<sprites.size(); i++) { // show the monsters
        fb.draw_rectangle(sprites[i].x*cell_w-3, sprites[i].y*cell_h-3, 6, 6, pack_color(255, 0, 0));
    }
}
/// <summary>
/// // Draw a sprite onto the framebuffer
/// </summary>
/// <param name="fb"></param>
/// <param name="sprite"></param>
/// <param name="depth_buffer"></param>
/// <param name="player"></param>
/// <param name="tex_sprites"></param>
void draw_sprite(FrameBuffer &fb, Sprite &sprite, const std::vector<float> &depth_buffer, const Player &player, const Texture &tex_sprites) {
    float sprite_dir = atan2(sprite.y - player.y, sprite.x - player.x);
    while (sprite_dir - player.a >  M_PI) sprite_dir -= 2*M_PI; 
    while (sprite_dir - player.a < -M_PI) sprite_dir += 2*M_PI;

    size_t sprite_screen_size = std::min(2000, static_cast<int>(fb.h/sprite.player_dist));
    int h_offset = (sprite_dir - player.a)*(fb.w/2)/(player.fov) + (fb.w/2)/2 - sprite_screen_size/2; 
    int v_offset = fb.h/2 - sprite_screen_size/2;

    for (size_t i=0; i<sprite_screen_size; i++) {
        if (h_offset+int(i)<0 || h_offset+i>=fb.w/2) continue;
        if (depth_buffer[h_offset+i]<sprite.player_dist) continue; 
        for (size_t j=0; j<sprite_screen_size; j++) {
            if (v_offset+int(j)<0 || v_offset+j>=fb.h) continue;
            uint32_t color = tex_sprites.get(i*tex_sprites.size/sprite_screen_size, j*tex_sprites.size/sprite_screen_size, sprite.tex_id);
            uint8_t r,g,b,a;
            unpack_color(color, r, g, b, a);
            if (a>128)
            fb.set_pixel(fb.w/2 + h_offset+i, v_offset+j, color);
        }
    }
}
/// <summary>
/// // Main render function that combines everything and draws the scene
/// </summary>
/// <param name="fb"></param>
/// <param name="gs"></param>
void render(FrameBuffer &fb, GameState &gs){
    const Map &map                     = gs.map;
    const Player &player               = gs.player;
    std::vector<Sprite> &sprites       = gs.monsters;
    const Texture &tex_walls           = gs.tex_walls;
    const Texture &tex_monst           = gs.tex_monst;
    const Texture& tex_weapon          = gs.tex_weapon;
    
    fb.clear(pack_color(128, 128, 128)); // clear the screen

    const size_t cell_w = fb.w/(map.w*2); // size of one map cell on the screen
    const size_t cell_h = fb.h/map.h;
    std::vector<float> depth_buffer(fb.w/2, 1e3);

    for (size_t i=0; i<fb.w/2; i++) { // draw the visibility cone AND the "3D" view
        float angle = player.a-player.fov/2 + player.fov*i/float(fb.w/2);
        for (float t=0; t<20; t+=.01) { 
            float x = player.x + t*cos(angle);
            float y = player.y + t*sin(angle);
            fb.set_pixel(x*cell_w, y*cell_h, pack_color(190, 190, 190)); // draws the visibility cone

            if (map.is_empty(x, y)) continue;

            size_t texid = map.get(x, y); // our ray touches a wall, so draw the vertical column to create an illusion of 3D
            assert(texid<tex_walls.count);
            float dist = t*cos(angle-player.a);
            depth_buffer[i] = dist;
            size_t column_height = std::min(2000, int(fb.h/dist));
            int x_texcoord = wall_x_texcoord(x, y, tex_walls);
            std::vector<uint32_t> column = tex_walls.get_scaled_column(texid, x_texcoord, column_height);
            int pix_x = i + fb.w/2; // we are drawing at the right half of the screen, thus +fb.w/2
            for (size_t j=0; j<column_height; j++) { // copy the texture column to the framebuffer
                int pix_y = j + fb.h/2 - column_height/2;
                if (pix_y>=0 && pix_y<(int)fb.h) {
                    fb.set_pixel(pix_x, pix_y, column[j]);
                }
            }
            break;
        } // ray marching loop
    } // field of view ray sweeping

    draw_map(fb, sprites, tex_walls, map, cell_w, cell_h);

    for (size_t i=0; i<sprites.size(); i++) { // draw the sprites
        draw_sprite(fb, sprites[i], depth_buffer, player, tex_monst);
    }
    size_t cross_center_x = fb.w * 3 / 4; 
    size_t cross_center_y = fb.h / 2;
    size_t cross_size = 10; 
    /// <summary>
    /// Draw the crosshair
    /// </summary>

    for (int x = cross_center_x - cross_size ; x <= cross_center_x + cross_size ; ++x) 
    {
        fb.set_pixel(x, cross_center_y, pack_color(255, 255, 255));
    }
    for (int y = cross_center_y - cross_size ; y <= cross_center_y + cross_size ; ++y) 
    {
        fb.set_pixel(cross_center_x, y, pack_color(255, 255, 255));
    }



    size_t weapon_width = 32;
    size_t weapon_height = 32;
    
    // Draw the weapon
    int enlarged_width = 180;
    int enlarged_height = 180; 
    size_t weapon_x = (fb.w * 3 / 4) - (enlarged_width / 2);
    size_t weapon_y = (fb.h * 3 / 4) - (enlarged_height / 2);
    for (int i = 0; i < weapon_width; ++i) {
        for (int j = 0; j < weapon_height; ++j) {
            uint32_t color = tex_weapon.get(i, j, gs.shoot);
            uint8_t r, g, b, a;
            unpack_color(color, r, g, b, a);
            if (a == 0) continue;

            
            for (int k = 0; k < enlarged_width / weapon_width; ++k) {
                for (int l = 0; l < enlarged_height / weapon_height; ++l) {
                    fb.set_pixel(weapon_x + i * (enlarged_width / weapon_width) + k, weapon_y + j * (enlarged_height / weapon_height) + l, color);
                }
            }
        }
    }
    /// <summary>
    ///Draw the status bar
    /// </summary>
    size_t rect_width = 512;
    size_t rect_height = 60; 
    size_t rect_x = fb.w  - rect_width; 
    size_t rect_y = fb.h - rect_height ; 

 
    for (size_t i = 0; i < rect_width; ++i) {
        for (size_t j = 0; j < rect_height; ++j) {
            fb.set_pixel(rect_x + i, rect_y + j, pack_color(225, 191, 146));
        }
    }
   /// <summary>
   /// Handle player death
   /// </summary>
   if (gs.player.health <= 0)
   {
        fb.clear(pack_color(255, 0, 0));
    }
}

