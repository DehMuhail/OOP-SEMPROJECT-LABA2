#ifndef SPRITE_H
#define SPRITE_H
#include <iostream>
#include <cstdlib>

/// <summary>
/// Sprite class representing a game entity with position, texture ID, player distance, and health
/// </summary>
class Sprite {
public:
    float x, y;
    size_t tex_id;
    float player_dist;
    bool operator < (const Sprite& s) const;
    int health;
};
/// <summary>
/// Abstract class of decorator pattern for extending Sprite functionalities
/// </summary>
class SpriteDecorator:public Sprite
{
protected:
    Sprite& sprite;
public:
    SpriteDecorator(Sprite& spr) : sprite(spr) {}
    virtual bool IsNearPlayer() const = 0;
    virtual bool IsPlayerInside() const = 0;
    virtual bool IsDistNotNull() const = 0;
};

/// <summary>
///  EnemyLogic class extending SpriteDecorator for specific enemy logic
/// </summary>
class EnemyLogic : public SpriteDecorator
{
public:
    EnemyLogic(Sprite& spr) :SpriteDecorator(spr) {};
    bool IsNearPlayer() const override
    {
        return sprite.player_dist < 3;
    };
    bool IsPlayerInside() const override
    {
        return sprite.player_dist <= 0.2;
    }
    bool IsDistNotNull() const override
    {
        return sprite.player_dist != 0;
    }
};

#endif // SPRITE_H

