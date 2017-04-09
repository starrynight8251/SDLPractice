//
//  player.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//
#include <SDL2_image/SDL_image.h>
#include "ltexture.h"
#include "player.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int JOYSTICK_DEAD_ZONE;
extern bool checkCollision( SDL_Rect a, SDL_Rect b );

Player::Player()
{
    //Initialize the offsets
    mPosX = PLAYER_X;
    mPosY = PLAYER_Y;
    mDir  = DOWN;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Set collision box dimension
    mCollider.x = mPosX + PLAYER_COLLIDE_X;
    mCollider.y = mPosY + PLAYER_COLLIDE_Y;
    mCollider.w = PLAYER_COLLIDE_W;
    mCollider.h = PLAYER_COLLIDE_H;
    
    mPlayerTexture = new LTexture();
    mPlayerTexture->loadFromFile( "graphics/walk.png" );
    if( mPlayerTexture == NULL )
    {
        printf( "Failed to load walking animation texture!\n" );
    }
    else
    {
        mPlayerClips[ 0 ].x =   0;
        mPlayerClips[ 0 ].y =   0;
        mPlayerClips[ 0 ].w =  32;
        mPlayerClips[ 0 ].h =  32;
        
        mPlayerClips[ 1 ].x =  32;
        mPlayerClips[ 1 ].y =   0;
        mPlayerClips[ 1 ].w =  32;
        mPlayerClips[ 1 ].h =  32;
        
        mPlayerClips[ 2 ].x =  64;
        mPlayerClips[ 2 ].y =   0;
        mPlayerClips[ 2 ].w =  32;
        mPlayerClips[ 2 ].h =  32;
        
        mPlayerClips[ 3 ].x =  32;
        mPlayerClips[ 3 ].y =   0;
        mPlayerClips[ 3 ].w =  32;
        mPlayerClips[ 3 ].h =  32;
    }
}

Player::~Player()
{
    mPlayerTexture->free();
    mPlayerTexture = NULL;
}

void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT:
                mDir = LEFT;
                mVelX -= PLAYER_VEL;
                break;
            case SDLK_RIGHT:
                mDir = RIGHT;
                mVelX += PLAYER_VEL;
                break;
            case SDLK_UP:
                mDir = UP;
                mVelY -= PLAYER_VEL;
                break;
            case SDLK_DOWN:
                mDir = DOWN;
                mVelY += PLAYER_VEL;
                break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT: mVelX += PLAYER_VEL; break;
            case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
            case SDLK_UP: mVelY += PLAYER_VEL; break;
            case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
        }
    }
    // コントローラ十字キー
    else if( e.type == SDL_JOYAXISMOTION )
    {
        //　コントローラ１
        if( e.jaxis.which == 0 )
        {
            // コントローラ左右キー
            if( e.jaxis.axis == 0 )
            {
                // 左
                if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
                {
                    mDir = LEFT;
                    mVelX -= PLAYER_VEL;
                }
                // 右
                else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
                {
                    mDir = RIGHT;
                    mVelX += PLAYER_VEL;
                }
                // 左右ニュートラル
                else
                {
                    mVelX = 0;
                }
            }
            // コントローラ上下キー
            else if( e.jaxis.axis == 1 )
            {
                // 上
                if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
                {
                    mDir = UP;
                    mVelY -= PLAYER_VEL;
                }
                // 下
                else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
                {
                    mDir = DOWN;
                    mVelY += PLAYER_VEL;
                }
                // 上下ニュートラル
                else
                {
                    mVelY = 0;
                }
            }
        }
    }
    

}

void Player::move( SDL_Rect& wall )
{
    //Move the player left or right
    mPosX += mVelX;
	mCollider.x = mPosX + PLAYER_COLLIDE_X;

    //If the player collided or went too far to the left or right
    if( (mCollider.x < 0 ) || ( mCollider.x + mCollider.w > SCREEN_WIDTH ) || checkCollision( mCollider, wall ) )
    {
        //Move back
        mPosX -= mVelX;
		mCollider.x = mPosX + PLAYER_COLLIDE_X;
    }

    //Move the player up or down
   		mPosY += mVelY;
		mCollider.y = mPosY + PLAYER_COLLIDE_Y;

    //If the player collided or went too far up or down
    if( ( mCollider.y < 0 ) || ( mCollider.y + mCollider.h > SCREEN_HEIGHT ) || checkCollision( mCollider, wall ) )
    {
        //Move back
        mPosY -= mVelY;
		mCollider.y = mPosY + PLAYER_COLLIDE_Y;
    }

}

void Player::render( int frame )
{
    //Show the player
	SDL_Rect* currentClip = &mPlayerClips[ (frame % 16)/4 ];
	currentClip->y = mDir*32;
	mPlayerTexture->render( mPosX, mPosY, 1, currentClip );
}
