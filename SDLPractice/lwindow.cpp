//
//  lwindow.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/12.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "lwindow.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

LWindow::LWindow()
{
    mWindow = NULL;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullScreen = false;
    mMinimized = false;
    mWidth = 0;
    mHeight = 0;
}

bool LWindow::init()
{
    // ウィンドウ作成
    mWindow = SDL_CreateWindow( "starrynight8251", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( mWindow != NULL )
    {
        mMouseFocus = true;
        mKeyboardFocus = true;
        mWidth = SCREEN_WIDTH;
        mHeight = SCREEN_HEIGHT;
    }
    
    return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer()
{
    return SDL_CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
}

void LWindow::handleEvent( SDL_Event& e )
{
    // イベント判別処理
    if( e.type == SDL_WINDOWEVENT )
    {
        switch( e.window.event )
        {
            // ウィンドウサイズ変更
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                mWidth = e.window.data1;
                mHeight = e.window.data2;
                SDL_RenderPresent( gRenderer );
                break;
                
            // エクスポージャで再描画
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent( gRenderer );
                break;
                
            // マウスがウィンドウに入った
            case SDL_WINDOWEVENT_ENTER:
                mMouseFocus = true;
                break;
                
            // マウスがウィンドウから出た
            case SDL_WINDOWEVENT_LEAVE:
                mMouseFocus = false;
                break;
                
            // ウィンドウがキーボードフォーカスを取得した
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                mKeyboardFocus = true;
                break;
                
            // ウィンドウがキーボードフォーカスを失った
            case SDL_WINDOWEVENT_FOCUS_LOST:
                mKeyboardFocus = false;
                break;
                
            // ウィンドウが最小化された
            case SDL_WINDOWEVENT_MINIMIZED:
                mMinimized = true;
                break;
                
            // ウィンドウが最大化された
            case SDL_WINDOWEVENT_MAXIMIZED:
                mMinimized = false;
                break;
                
            // ウィンドウが復元された
            case SDL_WINDOWEVENT_RESTORED:
                mMinimized = false;
                break;
        }
        
    }
    
    // enterキーでウィンドウ最大化切替
    else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN )
    {
        if( mFullScreen )
        {
            SDL_SetWindowFullscreen( mWindow, SDL_FALSE );
            mFullScreen = false;
        }
        else
        {
            SDL_SetWindowFullscreen( mWindow, SDL_TRUE );
            mFullScreen = true;
            mMinimized = false;
        }
    }
}

void LWindow::free()
{
    if( mWindow != NULL )
    {
        SDL_DestroyWindow( mWindow );
    }
    
    mMouseFocus = false;
    mKeyboardFocus = false;
    mWidth = 0;
    mHeight = 0;
}

int LWindow::getWidth()
{
    return mWidth;
}

int LWindow::getHeight()
{
    return mHeight;
}

bool LWindow::hasMouseFocus()
{
    return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
    return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
    return mMinimized;
}

