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
    SDL_Texture* mGridText;
    SDL_Surface* mGridSurf;

    SDL_Surface* Surf_Screen;
    SDL_Surface* mXSurf;
    SDL_Surface* mOSurf;

    SDL_Renderer* mRenderer;

    CSurface* mSurface;

    enum GridType
    {
        GRID_TYPE_NONE = 0,
        GRID_TYPE_X,
        GRID_TYPE_O
    };

    GridType mGrid[9];

public:
    CApp();
    ~CApp();

public:
    void resetGrid();

    void setCell(int index, GridType type);

    bool onInit();

    bool onLoop();

    void onRender();

    void onCleanup();

    void onExit();

    void onResize(int w, int h);

    void onKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode);
};