/*! \mainpage OOP-SEM2-Project 
 *
 * \section intro_sec Introduction
 *
 * This project is a Doom-like game implemented in C++ using the SDL2 library.
 * It features a player navigating through a map filled with monsters, shooting enemies, and managing health.
 *
 * \section comp_sec Components:
 *
 * -SDL2: The Simple DirectMedia Layer is used for rendering graphics, handling input, and managing windows. <br>
 * -SDL_ttf: SDL TrueType Font library is used for rendering text.<br>
 * -SDL_mixer: SDL Mixer library is used for playing music and sound effects.<br>
 * 
 * \section comp_sec2 Shortsummary 
 * 
 * This project demonstrates various object-oriented design principles and patterns, used of treads and other.
 *
 *
 */



#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cmath>
#include "SDL.h"
#include <SDL_ttf.h>
#include "utils.h"
#include "raycaster.h"
#include "text.h"
#include "SDL_mixer.h"
#include "RandomMove.h"
//#include "random"

bool isShiftPressed = false;
int BASE_SPEED = 1;
int MAX_SPEED = 2;
bool isInvulnerable = false;
const Uint32 invulnerabilityDuration = 1000; 
Uint32 invulnerabilityStartTime = SDL_GetTicks();
char Mtimer[10];
bool reload = false;
const Uint32 reloadDuration = 1000;
Uint32 reloadMoment = SDL_GetTicks();
Uint32 timerUpdate = SDL_GetTicks();
Uint32 Time = SDL_GetTicks();
std::vector<std::thread> threads;
//Mix_Music* music;

struct GameState; 

/// <summary>
/// This class implements the Observer pattern and is responsible for observing changes in the player's health by 5 points; 
/// </summary>
class HealthObserver { 
public:
    void update(GameState* gs)
    {
        gs[0].player.health -= 5;
    }
};
HealthObserver damage;
/// <summary>
/// This class implements the  State pattern. Has two subclasses: reloadpattern and Invulnerablepattern
/// </summary>
class PlayerState
{
public:
    virtual ~PlayerState() {}
    virtual void handleInput() = 0;
};

/// <summary>
/// Handles the reloading mechanism after last shot 
/// </summary>

class reloadpattern :public PlayerState
{
public:
    void handleInput() override
    {
        if (reload && SDL_GetTicks() - reloadMoment >= reloadDuration)
        {
            reload = false;
        }
    }

};
reloadpattern reloadp;

/// <summary>
/// Manages the invulnerability state which gains after hit
/// </summary>

class Invulnerablepattern :public PlayerState
{
public:
    void handleInput() override
    {
        if (isInvulnerable && SDL_GetTicks() - invulnerabilityStartTime >= invulnerabilityDuration)
        {
            isInvulnerable = false;
        }
    }
};
Invulnerablepattern Invulp;

/// <summary>
/// This class implements the Singleton pattern and is responsible for managing game sounds.
/// </summary>

    class SoundManager
    {
    private:
        SoundManager() {}
        Mix_Music* music;

    public:
        static SoundManager& GetInstance()
        {
            static SoundManager instance;
            return instance;
        }

        void LoadMusic(const std::string& filename)
        {
            music = Mix_LoadMUS(filename.c_str());
            if (!music) {
                std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
            }
        }

        Mix_Music* getMusic() const {
            return music;
        }

        void PlayMusic(int times)
        {
            Mix_PlayMusic(music, times);
        }
    };
    SoundManager& sound = SoundManager::GetInstance();

/// <summary>
/// This class defines the core game logic and serves as an abstract base class. Template pattern. <br> Has a subclass: DoomGame
/// </summary>
class Game
{
public:
    virtual int InternalGameLogic(GameState* gs) = 0;
    virtual int PlayerControls(GameState* gs) = 0;
    virtual int GameRender(GameState* gs, FrameBuffer& fb, SDL_Texture* framebuffer_texture, SDL_Renderer* renderer, TTF_Font* font) = 0;

    ///Game Cycle
    void run(GameState* gs, FrameBuffer& fb, SDL_Texture* framebuffer_texture, SDL_Renderer* renderer, TTF_Font* font)
    {
        while (true)
        {
            if (!InternalGameLogic(gs)) continue;
            if(!PlayerControls(gs)) break;
            GameRender(gs, fb, framebuffer_texture, renderer, font);
        }

    }
};

/// <summary>
///  Implements specific game logic, such as internal game logic, player controls, and rendering.
/// </summary>

class DoomGame :public Game
{
public: 
    int InternalGameLogic(GameState* gs) override
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        if (gs[0].monsters.size() == 0)
        {
            for (auto& t:threads)
            {
                t.join();
            }
            threads.clear();
            gs[1].player.health = gs[0].player.health;
            gs[0] = std::move(gs[1]);
            for (int i = 0; i < gs[0].monsters.size(); i++)
            {
                threads.emplace_back(RandomMonMove, std::ref(gs[0].monsters[i]), std::ref(gs[0].player),std::ref(gs[0].map));
            }
            
        }
        Uint32 t = SDL_GetTicks();
        if ((t - timerUpdate) >= 1000)
        {
            timertime((t - Time) / 1000, Mtimer);
            timerUpdate = SDL_GetTicks();
        }
        reloadp.handleInput();
        Invulp.handleInput();

        /*
        if (isInvulnerable && SDL_GetTicks() - invulnerabilityStartTime >= invulnerabilityDuration)
        {
            isInvulnerable = false;
        }
        */
        /*
        if (reload && SDL_GetTicks() - reloadMoment >= reloadDuration)
        {
            reload = false;
        }
        */
        EnemyLogic EL(gs[0].monsters[0]);
        for (int i = 0; i < gs[0].monsters.size(); i++)
        {
            for (size_t i = 0; i < gs[0].monsters.size(); i++) { // update the distances from the player to each sprite
                gs[0].monsters[i].player_dist = std::sqrt(pow(gs[0].player.x - gs[0].monsters[i].x, 2) + pow(gs[0].player.y - gs[0].monsters[i].y, 2));
            }
            //EL.SetSprite(gs[0].monsters[i]);
            EnemyLogic EL(gs[0].monsters[i]);
            if (/*gs[0].monsters[i].player_dist < 3 */ /**/EL.IsNearPlayer())
            {

                float nx = gs[0].monsters[i].x + (gs[0].player.x - gs[0].monsters[i].x) * 0.02;
                float ny = gs[0].monsters[i].y + (gs[0].player.y - gs[0].monsters[i].y) * 0.02;

                if (int(nx) >= 0 && int(nx) < int(gs[0].map.w) && int(ny) >= 0 && int(ny) < int(gs[0].map.h)) {
                    if (gs[0].map.is_empty(nx, gs[0].monsters[i].y)) gs[0].monsters[i].x = nx;
                    if (gs[0].map.is_empty(gs[0].monsters[i].x, ny)) gs[0].monsters[i].y = ny;
                }
            }
            //else  
            if (EL.IsPlayerInside() && EL.IsDistNotNull() /*gs[0].monsters[i].player_dist <= 0.2 && gs[0].monsters[i].player_dist != 0*/) {

                if (!isInvulnerable) {
                    damage.update(gs);
                    //gs[0].player.health -= gs[0].monsters[i].hit();
                    //std::cout << gs[0].player.health << std::endl;
                    isInvulnerable = true;
                    invulnerabilityStartTime = SDL_GetTicks();
                }
            }
        }
  

        {
            gs[0].player.a += float(gs[0].player.turn) * .05; 
            float nx = gs[0].player.x + gs[0].player.walk * cos(gs[0].player.a) * .05;
            float ny = gs[0].player.y + gs[0].player.walk * sin(gs[0].player.a) * .05;

            if (int(nx) >= 0 && int(nx) < int(gs[0].map.w) && int(ny) >= 0 && int(ny) < int(gs[0].map.h)) {
                if (gs[0].map.is_empty(nx, gs[0].player.y)) gs[0].player.x = nx;
                if (gs[0].map.is_empty(gs[0].player.x, ny)) gs[0].player.y = ny;
            }
           
            std::sort(gs[0].monsters.begin(), gs[0].monsters.end()); // sort it from farthest to closest
        }
        return 1;
    }
    int PlayerControls(GameState* gs) override
    {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (SDL_QUIT == event.type || (SDL_KEYDOWN == event.type && SDLK_ESCAPE == event.key.keysym.sym)) 
            { 
                for (int i = 0; i < gs[0].monsters.size(); i++)
                {
                    gs[0].monsters[i].health = 0;
                    SDL_Delay(200);
                }
                return 0;
            }
            if (SDL_KEYUP == event.type) {
                if ('a' == event.key.keysym.sym || 'd' == event.key.keysym.sym) gs[0].player.turn = 0;
                if ('w' == event.key.keysym.sym || 's' == event.key.keysym.sym) gs[0].player.walk = 0;
                if (' ' == event.key.keysym.sym) gs[0].shoot = false;
                if (SDLK_LSHIFT == event.key.keysym.sym)
                {
                    isShiftPressed = false;
                    gs[0].player.walk = BASE_SPEED;
                }
            }
            if (SDL_KEYDOWN == event.type) {
                if ('a' == event.key.keysym.sym) gs[0].player.turn = -1;
                if ('d' == event.key.keysym.sym) gs[0].player.turn = 1;
                if ('w' == event.key.keysym.sym) gs[0].player.walk = BASE_SPEED;
                if ('s' == event.key.keysym.sym) gs[0].player.walk = -BASE_SPEED;
                if (' ' == event.key.keysym.sym && !reload)
                {
                    sound.PlayMusic(1);
                    //Mix_PlayMusic(music, 1);
                    std::cout << "shoot" << std::endl;
                    gs[0].player.shoot(gs[0].monsters, gs[0].map);
                    reload = true;
                    reloadMoment = SDL_GetTicks();
                    gs[0].shoot = true;
                }

                if (SDLK_LSHIFT == event.key.keysym.sym) isShiftPressed = true;
            }
            if (isShiftPressed) {
                gs[0].player.walk *= 2;
                if (gs[0].player.walk > MAX_SPEED)
                {
                    gs[0].player.walk = MAX_SPEED;
                }
                if (gs[0].player.walk < -MAX_SPEED)
                {
                    gs[0].player.walk = -MAX_SPEED;
                }
            }
        }
        return 1;
    }
    int GameRender(GameState* gs, FrameBuffer& fb, SDL_Texture* framebuffer_texture, SDL_Renderer* renderer, TTF_Font* font) override
    {
        Text* str = new Text(renderer, font);
        Text* str2 = new Text(renderer, font);
        render(fb, gs[0]);
        SDL_UpdateTexture(framebuffer_texture, NULL, reinterpret_cast<void*>(fb.img.data()), fb.w * 4);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, framebuffer_texture, NULL, NULL);
        if (gs[0].player.health > 0)
        {
            char health[4];
            str->setText(std::string(Mtimer), fb.w / 2, 512 - 60);
            str->render();
            str2->setText(std::to_string(gs[0].player.health), fb.w * 3 / 4, 512 - 60);
            str2->render();

        }
        else
        {
            str->setText("Game over", fb.w / 2 - 120, fb.h * 1 / 3);
            str->render();
        }

        SDL_RenderPresent(renderer);
        return 1;
    }
};


/// <summary>
///Initializes SDL2, SDL_ttf, and SDL_mixer libraries.
///Sets up game components such as the window, renderer, and textures.
///Creates instances of the game stateand runs the game loop.
///Manages threads for monster movement.
///Cleans up resources before exiting.
/// </summary>
/// <returns></returns>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (TTF_Init() == -1) {
        printf("[ERROR] TTF_Init() Failed with: %s\n", TTF_GetError());
        exit(2);
    }
    TTF_Font* font = TTF_OpenFont("OpenSans-Bold.ttf", 48);
    if (font == NULL) {
        printf("Failed to load font: %s", SDL_GetError());
    }
    int res;
    if (res = Mix_Init(MIX_INIT_WAVPACK) == 0) {
        std::cout << "Quit " << res << std::endl;
        SDL_Quit();
        Mix_Quit();
        return 1;
    }
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    //sound = SoundManager::GetInstance();
    sound.LoadMusic("shot.wav");
    //music = Mix_LoadMUS("shot.wav");

    FrameBuffer fb{ 1024, 512, std::vector<uint32_t>(1024 * 512, pack_color(255, 255, 255)) };
    GameState gsm[2] = {
        { Map(1, 16 ,16),                                // game map
                  {3.456, 2.345, 1.523, M_PI / 3., 0, 0, 15}, // player
                  { {8.523, 1.812, 2, 0, 3},               // monsters lists
                    },
                TextureFactory::createTexture("walltext1.bmp", SDL_PIXELFORMAT_ABGR8888),  // textures for the walls
                TextureFactory::createTexture("monsters2.bmp", SDL_PIXELFORMAT_ABGR8888), // textures for the monsters
                TextureFactory::createTexture("gun.bmp", SDL_PIXELFORMAT_ABGR8888) },
        {Map(2, 18, 18),                                // game map
                  {1.5, 2.0, 1.523, M_PI / 3., 0, 0, 100}, // player
                  { {3.523, 3.812, 2, 0, 3},               // monsters lists
                    {1.834, 8.765, 0, 0, 3},
                    {5.323, 5.365, 1, 0, 3},
                    {14.32, 13.36, 3, 0, 3},
                    {4.123, 10.76, 1, 0, 3} },
                  TextureFactory::createTexture("walltext2.bmp", SDL_PIXELFORMAT_ABGR8888),  // textures for the walls
                  TextureFactory::createTexture("monsters2.bmp", SDL_PIXELFORMAT_ABGR8888), // textures for the monsters
                  TextureFactory::createTexture("gun.bmp", SDL_PIXELFORMAT_ABGR8888) }

    };


    for (int i = 0; i < gsm[0].monsters.size(); i++)
    {
        threads.emplace_back(RandomMonMove, std::ref(gsm[0].monsters[i]), std::ref(gsm[0].player),std::ref(gsm[0].map));
    }


    if (!gsm[0].tex_walls.count || !gsm[0].tex_monst.count) {
        std::cerr << "Failed to load textures" << std::endl;
        return -1;
    }
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (SDL_CreateWindowAndRenderer(fb.w, fb.h, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
        std::cerr << "Failed to create window and renderer: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Texture* framebuffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, fb.w, fb.h);
    if (!framebuffer_texture) {
        std::cerr << "Failed to create framebuffer texture : " << SDL_GetError() << std::endl;
        return -1;
    }


    
    if (!gsm[0].tex_walls.count || !gsm[0].tex_monst.count) {
        std::cerr << "Failed to load textures" << std::endl;
        return -1;
    }


    DoomGame dg;
    dg.run(gsm, fb, framebuffer_texture, renderer, font);

    for (auto& t : threads)
    {
        t.join();
    }

    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
   // delete str;
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

