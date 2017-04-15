//
//  lbutton.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/01.
//  Copyright © 2017年 NoCompany. All rights reserved.
//
#include "ltexture.h"
#include "lbutton.h"

extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;
extern const int TOTAL_BUTTONS;

extern LTexture* gButtonTexture;
extern SDL_Rect gButtonClips[ BUTTON_SPRITE_TOTAL ];

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;
    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //　マウスの位置を取得する
        int x, y;
        SDL_GetMouseState( &x, &y );
        
        // マウスがボタンの内側にあるか判定する
        bool inside = true;
        
        // マウスがボタンの左側にある
        if( x < mPosition.x )
        {
            inside = false;
        }
        // マウスがボタンの右側にある
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        // マウスがボタンの上側にある
        else if( y < mPosition.y )
        {
            inside = false;
        }
        // マウスがボタンの下側にある
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }
        
        // マウスがボタンの外側にある
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        // マウスがボタンの上にある
        else
        {
            // マウスイベントに応じてスプライト設定
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                    break;
            }
        }
    }
}

void LButton::render()
{
    // マウス用ボタン描画
    gButtonTexture->render( mPosition.x, mPosition.y, &gButtonClips[ mCurrentSprite ] );
}


