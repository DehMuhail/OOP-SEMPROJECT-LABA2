#pragma once
#include "sprite.h"
#include "Player.h"
#include "random"

std::random_device r;
std::default_random_engine e1(r());
std::uniform_int_distribution<int> uniform_dist(-1, 1);
void RMMbody(Sprite& spr, Map& map)
{
    float tx = spr.x;
    float ty = spr.y;
    float nx = spr.x + uniform_dist(e1) * 0.02;
    float ny = spr.y + uniform_dist(e1) * 0.02;
    spr.x = nx;
    spr.y = ny;
    
    //if commented, unittest1 can be failed 
    
    if (int(nx) >= 0 && int(nx) < int(map.w) && int(ny) >= 0 && int(ny) < int(map.h)) {
        if (map.is_empty(nx, spr.y)) spr.x = nx;
        else spr.x = tx;
        if (map.is_empty(spr.x, ny)) spr.y = ny;
        else spr.y = ty;
    }
    
}
/// <summary>
/// A helper function for implementing random movement for monsters
/// </summary>
/// <param name="spr"></param>
/// <param name="plr"></param>
/// <param name="map"></param>

void RandomMonMove(Sprite& spr, Player& plr, Map& map)
{                                   //To fix unit-test 

    while (plr.health > 0 && spr.health > 0)
    { 
        RMMbody(spr, map); 
        SDL_Delay(100);
    }
    std::cout << "Thread is ended" << std::endl;
}

