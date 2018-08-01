#include "CApp.h"
#include "CSurface.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL_image.h>

#include <iostream>



CApp::CApp()
        : mWindow { NULL }
        , mTexture { NULL }
        , mGrid { NULL }
        , mX { NULL }
        , mO { NULL }
        , mRenderer{ NULL }
        , mSurface{ NULL }
{
    mIsRunning = true;
    mSurface = new CSurface();
}

CApp::~CApp(){}

bool CApp::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    mWindow = SDL_CreateWindow(TITLE,
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_RESIZABLE);

    if(mWindow == NULL) {
        std::cout << "SDL_CreateWindow got NULL!" << std::endl;
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, 0);

    if (mRenderer == NULL) {
        std::cout << "SDL_CreateRenderer got NULL!" << std::endl;
        return false;
    }

    std::string img = "./assets/astronaut.png";
    mTexture = CSurface::loadTexture(mRenderer, img);
    if(mTexture == NULL) {
        std::cout << "Could not loadTexture!" << std::endl;
        return false;
    }

    std::string gridAsset = "./assets/grid.png";
    if((mGrid = CSurface::loadTexture(mRenderer, gridAsset)) == NULL) {
        std::cout << "Could not load asset!" << gridAsset << std::endl;
        return false;
    }

    std::string xAsset = "./assets/x.png";
    if((mX = CSurface::loadSurface(mRenderer, xAsset)) == NULL) {
        std::cout << "Could not load asset!" << xAsset << std::endl;
        return false;
    }

    std::string oAsset = "./assets/o.png";
    if((mO = CSurface::loadSurface(mRenderer, oAsset)) == NULL) {
        std::cout << "Could not load asset" << oAsset << std::endl;
        return false;
    }

    mSurface->Transparent(mX, 255, 0, 255);
    mSurface->Transparent(mO, 255, 0, 255);


    SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 0); // green
    SDL_RenderClear(mRenderer);
    SDL_RenderPresent(mRenderer);

    return true;
}

bool CApp::onLoop() {
    return mIsRunning;
}

void CApp::onRender() {
    SDL_RenderClear( mRenderer );
    //CSurface::OnDraw(mRenderer, mTexture, 0, 0);
    SDL_RenderCopy(mRenderer, mGrid, NULL, NULL);

    SDL_RenderPresent(mRenderer);
    SDL_Delay(1000 / FRAMES_PER_SECOND);
}

void CApp::onCleanup() {
    delete mSurface;
    SDL_DestroyTexture(mTexture);
    SDL_DestroyTexture(mGrid);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    std::cout << "Quitting..." << std::endl;
}

void CApp::OnExit() {
    std::cout << "Quiting... bye!" << std::endl;
    mIsRunning = false;
}

void CApp::OnResize(int w,int h) {
    std::cout << "Window resized width: " << w << ", height: " << h << std::endl;
}

void CApp::OnKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode)
{
    std::cout << "Key pressed: " << unicode << std::endl;
}
