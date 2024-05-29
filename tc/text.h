#pragma once
#include "SDL.h"
#include <SDL_ttf.h>
#include <iostream>
class Text {
    SDL_Texture* texture = NULL;
    SDL_Rect position;
    TTF_Font* font;
    SDL_Renderer* renderer;
    SDL_Color color;
    std::string text;
public:
    Text(SDL_Renderer* renderer, TTF_Font* font, std::string text = "", int x = 0, int y = 0, SDL_Color color = { 255, 255, 255 }) {
        position.x = x;
        position.y = y;
        this->color = color;
        this->font = font;
        this->renderer = renderer;
    }

    void setText(std::string text, int x, int y) {
        if (this->text == text) {
            return;
        }
        this->text = text;
        position.x = x;
        position.y = y;
        SDL_DestroyTexture(texture);
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (surface == NULL) {
            printf("Failed to render text: %s", SDL_GetError());
        }
        position.w = surface->w;
        position.h = surface->h;
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    void render() {
        SDL_RenderCopy(renderer, texture, NULL, &position);
    }

    ~Text() {
        SDL_DestroyTexture(texture);
    }


};

void timertime(Uint32 time, char tt[])
{

    //timer = hh/mm/ss
    Uint32 hh, mm, ss;
    ss = time;

    mm = ss / 60;
    ss = ss % 60;

    hh = mm / 60;
    mm = mm % 60;

    tt[0] = hh / 10 + '0';
    tt[1] = hh % 10 + '0';
    tt[2] = ':';
    tt[3] = mm / 10 + '0';
    tt[4] = mm % 10 + '0';
    tt[5] = ':';
    tt[6] = ss / 10 + '0';
    tt[7] = ss % 10 + '0';
    tt[8] = '\0';
}
