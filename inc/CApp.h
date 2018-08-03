#pragma once

#include "CEvent.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

/// Forward declarations
class CSurface;


// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int CELL_WIDTH = 200;

const int FRAMES_PER_SECOND = 2;
const char TITLE[] = "Tic Tac Toe";

// Colors
const SDL_Color WHITE = { 255, 255, 255 };
const SDL_Color RED = { 255, 0, 0 };
const SDL_Color GREEN = { 0, 255, 0 };
const SDL_Color BLUE = { 0, 0, 255 };


class CApp : public CEvent
{
private:
    bool mIsRunning;

    SDL_Window* Surf_Display;
    SDL_Surface* Surf_Screen;

    SDL_Surface* Surf_Grid;
    SDL_Surface* Surf_X;
    SDL_Surface* Surf_O;

    SDL_Surface* surfaceMessage; // Text surface
    TTF_Font* mFont;
    std::string mMessage;

    SDL_Renderer* mRenderer;

    CSurface* mSurface;

    enum GridType
    {
        GRID_TYPE_NONE = 0,
        GRID_TYPE_X,
        GRID_TYPE_O
    };

    GridType mGrid[9];
    int CurrentPlayer;

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

    void onLButtonDown(int x, int y);

    void onResize(int w, int h);

    void onKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode);
};