#include "CApp.h"
#include "CSurface.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL_image.h>

#include <iostream>


CApp::CApp()
: mIsRunning{ true }
, Surf_Display{ NULL }
, Surf_Screen { NULL}
, Surf_Grid { NULL }
, Surf_X{ NULL }
, Surf_O{ NULL }
, mRenderer{ NULL }
, mSurface{ new CSurface() }
, CurrentPlayer{ 0 }
{
}

CApp::~CApp()
{
}

void CApp::resetGrid()
{
    for(int i = 0;i < 9;i++)
    {
        mGrid[i] = GRID_TYPE_NONE;
    }
}

void CApp::setCell(int index, GridType type)
{
    if(index < 0 || index >= 9)
    {
        return;
    }
    if(type < 0 || type > GRID_TYPE_O)
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

    Surf_Display = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (Surf_Display == NULL)
    {
        std::cout << "SDL_CreateWindow got NULL!" << std::endl;
        return false;
    }


    Surf_Screen = SDL_GetWindowSurface(Surf_Display);

    Surf_Grid = CSurface::OnLoad((char *)"./assets/grid.bmp");
    if ( Surf_Grid == NULL )
    {
        printf("Loading Image failed: %s\n", SDL_GetError());
        return false;
    }

    Surf_X = CSurface::OnLoad((char *)"./assets/x.bmp");
    if ( Surf_X == NULL )
    {
        printf("Loading Image failed: %s\n", SDL_GetError());
        return false;
    }

    Surf_O = CSurface::OnLoad((char *)"./assets/o.bmp");
    if ( Surf_O == NULL )
    {
        printf("Loading Image failed: %s\n", SDL_GetError());
        return false;
    }

    mSurface->Transparent(Surf_X, 255, 0, 255);
    mSurface->Transparent(Surf_O, 255, 0, 255);

    resetGrid();

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
    for(int i = 0; i < 9; i++) {
        int X = (i % 3) * 200;
        int Y = (i / 3) * 200;

        if (mGrid[i] == GRID_TYPE_X) {
            CSurface::OnDraw(Surf_Screen, Surf_X, X, Y);
        }
        else if (mGrid[i] == GRID_TYPE_O)
        {
                CSurface::OnDraw(Surf_Screen, Surf_O, X, Y);
        }
    }
    SDL_UpdateWindowSurface(Surf_Display);

    SDL_Delay(1000 / FRAMES_PER_SECOND);
}

void CApp::onCleanup()
{
    SDL_FreeSurface(Surf_X);
    SDL_FreeSurface(Surf_O);
    SDL_FreeSurface(Surf_Grid);

    SDL_FreeSurface(Surf_Screen);
    SDL_DestroyWindow(Surf_Display);

    delete mSurface;


    SDL_Quit();
    std::cout << "Quitting..." << std::endl;
}

void CApp::onExit()
{
    std::cout << "Quiting... bye!" << std::endl;
    mIsRunning = false;
}

void CApp::onLButtonDown(int x, int y) {
    int ID    = x / 200;
    ID = ID + ((y / 200) * 3);

    if(mGrid[ID] != GRID_TYPE_NONE) {
        return;
    }

    if(CurrentPlayer == 0) {
        setCell(ID, GRID_TYPE_X);
        CurrentPlayer = 1;
    }
    else
    {
        setCell(ID, GRID_TYPE_O);
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
