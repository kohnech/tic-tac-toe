#include "CApp.h"
#include "CSurface.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL_image.h>

#include <iostream>


CApp::CApp()
: mIsRunning{ true }
, mWindow{ NULL }
, mTexture{ NULL }
, mGridText{ NULL }
, mXSurf{ NULL }
, mOSurf{ NULL }
, mRenderer{ NULL }
, mSurface{ new CSurface() }
{
}

CApp::~CApp()
{
    onCleanup();
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
        return false;
    }

    mWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (mWindow == NULL)
    {
        std::cout << "SDL_CreateWindow got NULL!" << std::endl;
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, 0);

    if (mRenderer == NULL)
    {
        std::cout << "SDL_CreateRenderer got NULL!" << std::endl;
        return false;
    }

    std::string img = "./assets/astronaut.png";
    mTexture = CSurface::loadTexture(mRenderer, img);
    if (mTexture == NULL)
    {
        std::cout << "Could not loadTexture!" << std::endl;
        return false;
    }

    std::string gridAsset = "./assets/grid.png";
    if ((mGridText = CSurface::loadTexture(mRenderer, gridAsset)) == NULL)
    {
        std::cout << "Could not load asset!" << gridAsset << std::endl;
        return false;
    }

    std::string xAsset = "./assets/x.png";
    if ((mXSurf = CSurface::loadSurface(xAsset)) == NULL)
    {
        std::cout << "Could not load asset!" << xAsset << std::endl;
        return false;
    }

    std::string oAsset = "./assets/o.png";
    if ((mOSurf = CSurface::loadSurface(oAsset)) == NULL)
    {
        std::cout << "Could not load asset" << oAsset << std::endl;
        return false;
    }

    mSurface->Transparent(mXSurf, 255, 0, 255);
    mSurface->Transparent(mOSurf, 255, 0, 255);


    SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 0); // green
    SDL_RenderClear(mRenderer);
    SDL_RenderPresent(mRenderer);

    resetGrid();

    return true;
}

bool CApp::onLoop()
{
    return mIsRunning;
}

void CApp::onRender()
{
    SDL_RenderClear(mRenderer);
    // CSurface::OnDraw(mRenderer, mTexture, 0, 0);
    // Render Grid
    for(int i = 0; i < 9; i++) {
        int X = (i % 3) * 200;
        int Y = (i / 3) * 200;

        if(mGrid[i] == GRID_TYPE_X)
        {
            CSurface::OnDraw(Surf_Display, Surf_X, X, Y);
        }
        else {
            if (mGrid[i] == GRID_TYPE_O)
            {
                CSurface::OnDraw(Surf_Display, Surf_O, X, Y);
            }
        }

    SDL_RenderCopy(mRenderer, mGridText, NULL, NULL);
    SDL_RenderPresent(mRenderer);
    SDL_Delay(1000 / FRAMES_PER_SECOND);
}

void CApp::onCleanup()
{
    delete mSurface;
    SDL_DestroyTexture(mTexture);
    SDL_DestroyTexture(mGridText);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    std::cout << "Quitting..." << std::endl;
}

void CApp::onExit()
{
    std::cout << "Quiting... bye!" << std::endl;
    mIsRunning = false;
}

void CApp::onResize(int w, int h)
{
    std::cout << "Window resized width: " << w << ", height: " << h << std::endl;
}

void CApp::onKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode)
{
    std::cout << "Key pressed: " << unicode << std::endl;
}
