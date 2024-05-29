#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <vector>

#include "map.h"
#include "player.h"
#include "sprite.h"
#include "framebuffer.h"
#include "textures.h"
/// <summary>
/// Game state struct holding game components
/// </summary>
struct GameState {
    Map map;
    Player player;
    std::vector<Sprite> monsters;
    Texture tex_walls;
    Texture tex_monst;
    Texture tex_weapon;
    bool shoot = false;

    GameState& operator=(GameState&& gs)
    {
        this->shoot = gs.shoot;
        this->map = gs.map;
        this->player = gs.player;
        this->monsters = gs.monsters;
        this->tex_walls = gs.tex_walls;
        this->tex_monst = gs.tex_monst;
        this->tex_weapon = gs.tex_weapon;
        this->monsters = gs.monsters;
        return* this;
    }
};
/// <summary>
/// Function declarations
/// </summary>
/// <param name="fb"></param>
/// <param name="gs"></param>
void render(FrameBuffer &fb, GameState &gs);

#endif 

