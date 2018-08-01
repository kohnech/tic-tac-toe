#pragma once

#include "CEvent.h"

#include <SDL2/SDL.h>

#include <string>

/// Forward declarations
class CSurface;


// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

const int FRAMES_PER_SECOND = 2;
const char TITLE[] = "Tic Tac Toe";


class CApp : public CEvent
{
private:
    bool mIsRunning;

    SDL_Window* mWindow;

    SDL_Texture* mTexture;
    SDL_Texture* mGrid;
    SDL_Surface* mX;
    SDL_Surface* mO;

    SDL_Renderer* mRenderer;

    CSurface* mSurface;

public:
    CApp();
    ~CApp();

public:
    bool onInit();

    bool onLoop();

    void onRender();

    void onCleanup();

    void onExit();

    void onResize(int w, int h);

    void onKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode);
};