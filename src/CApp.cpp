#include "CApp.h"
#include "CSurface.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>


CApp::CApp()
: mIsRunning{ true }
, Surf_Display{ NULL }
, Surf_Screen{ NULL }
, Surf_Grid{ NULL }
, Surf_X{ NULL }
, Surf_O{ NULL }
, surfaceMessage{ NULL }
, mFont{ NULL }
, mRenderer{ NULL }
, mSurface{ new CSurface() }
, CurrentPlayer{ 0 }
{
}

CApp::~CApp()
{
    onCleanup();
}

void CApp::resetGrid()
{
    for (int i = 0; i < 9; i++)
    {
        mGrid[i] = GRID_TYPE_NONE;
    }
}

void CApp::setCell(int index, GridType type)
{
    if (index < 0 || index >= 9)
    {
        return;
    }
    if (type < 0 || type > GRID_TYPE_O)
    {
        return;
    }

    mGrid[index] = type;
}

bool CApp::onInit()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() < 0) {
        printf("TTF_Init failed: %s\n", SDL_GetError());
    }

    Surf_Display = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (Surf_Display == NULL)
    {
        std::cout << "SDL_CreateWindow got NULL!" << std::endl;
        return false;
    }


    Surf_Screen = SDL_GetWindowSurface(Surf_Display);

    Surf_Grid = CSurface::OnLoad((char*)"./assets/grid.bmp");
    if (Surf_Grid == NULL)
    {
        printf("Loading Image failed: %s\n", SDL_GetError());
        return false;
    }

    Surf_X = CSurface::OnLoad((char*)"./assets/x.bmp");
    if (Surf_X == NULL)
    {
        printf("Loading Image failed: %s\n", SDL_GetError());
        return false;
    }

    Surf_O = CSurface::OnLoad((char*)"./assets/o.bmp");
    if (Surf_O == NULL)
    {
        printf("Loading Image failed: %s\n", SDL_GetError());
        return false;
    }

    mSurface->Transparent(Surf_X, 255, 0, 255);
    mSurface->Transparent(Surf_O, 255, 0, 255);

    resetGrid();

    // Font stuff
    mFont = TTF_OpenFont("./assets/Ubuntu-C.ttf", 25);

    if (mFont < 0) {
        printf("Could not load fond TTF_OpenFont");
    }

    mMessage = "Welcome to Tic Tac Toe!";

    return true;
}

bool CApp::onLoop()
{
    return mIsRunning;
}

void CApp::onRender()
{
    CSurface::OnDraw(Surf_Screen, Surf_Grid, 0, 0);
    // Render Grid
    for (int i = 0; i < 9; i++)
    {
        int X = (i % 3) * CELL_WIDTH;
        int Y = (i / 3) * CELL_WIDTH;

        if (mGrid[i] == GRID_TYPE_X)
        {
            CSurface::OnDraw(Surf_Screen, Surf_X, X, Y);
        }
        else if (mGrid[i] == GRID_TYPE_O)
        {
            CSurface::OnDraw(Surf_Screen, Surf_O, X, Y);
        }
    }
    surfaceMessage = TTF_RenderUTF8_Solid(mFont, mMessage.c_str(), BLUE);
    CSurface::OnDraw(Surf_Screen, surfaceMessage, 0, 0);
    SDL_UpdateWindowSurface(Surf_Display);

    SDL_Delay(1000 / FRAMES_PER_SECOND);
}

void CApp::onCleanup()
{
    SDL_FreeSurface(Surf_X);
    SDL_FreeSurface(Surf_O);
    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(mFont);
    SDL_FreeSurface(Surf_Grid);

    SDL_FreeSurface(Surf_Screen);
    SDL_DestroyWindow(Surf_Display);

    delete mSurface;

    TTF_Quit();
    SDL_Quit();
    std::cout << "Quitting..." << std::endl;
}

void CApp::onExit()
{
    std::cout << "Quiting... bye!" << std::endl;
    mIsRunning = false;
}

void CApp::onLButtonDown(int x, int y)
{
    int ind = x / CELL_WIDTH;
    ind = ind + ((y / CELL_WIDTH) * 3);

    if (mGrid[ind] != GRID_TYPE_NONE)
    {
        return;
    }

    if (CurrentPlayer == 0)
    {
        mMessage = "Player2";
        setCell(ind, GRID_TYPE_X);
        CurrentPlayer = 1;
    }
    else
    {
        mMessage = "Player1";
        setCell(ind, GRID_TYPE_O);
        CurrentPlayer = 0;
    }
}

void CApp::onResize(int w, int h)
{
    std::cout << "Window resized width: " << w << ", height: " << h << std::endl;
}

void CApp::onKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode)
{
    std::cout << "Key pressed: " << unicode << std::endl;
}
