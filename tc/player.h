#ifndef PLAYER_H
#define PLAYER_H
#include "sprite.h"
#include <vector>
#include "map.h"
#include "SDL.h"
struct Player {
    float x, y; // position
    float a;    // view direction
    float fov;  // field of view
    int turn, walk;
    int health;

    void shoot(std::vector<Sprite>& v, Map &map)
    {
        float x1;
        float y1;
        float c = 0;
        for (; c < 20; c += .05) {
            x1 = x + c * cos(a);
            y1 = y + c * sin(a);
            if (!map.is_empty(x1,y1)) break;
            for (int i = 0; i < v.size(); i++)
            {
                if ((x1 - v[i].x) * (x1 - v[i].x) + (y1 - v[i].y) * (y1 - v[i].y) <= 0.1)
                {
                    v[i].health--;
                    std::cout << "enemy " << i << " is hitted" << std::endl; 
                    if (v[i].health <= 0)
                    {
                        v[i].health = 0;
                        SDL_Delay(30);
                        std::cout << "enemy " << i << " is dead" << std::endl;
                        v.erase(v.begin() + i);
                    }
                    return;
                }

            }
        }

        
    }

};

#endif // PLAYER_H

