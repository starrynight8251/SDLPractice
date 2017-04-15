//
//  dot.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "ltexture.h"
#include "dot.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern LTexture* gDotTexture;
extern bool checkCollision( SDL_Rect a, SDL_Rect b );

Dot::Dot()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;    

    //Set collision box dimension
    mCollider.w = DOT_WIDTH;
    mCollider.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move( SDL_Rect& wall )
{
    //Move the dot left or right
    mPosX += mVelX;
		mCollider.x = mPosX;

    //If the dot collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision( mCollider, wall ) )
    {
        //Move back
        mPosX -= mVelX;
				mCollider.x = mPosX;
    }

    //Move the dot up or down
    mPosY += mVelY;
		mCollider.y = mPosY;

    //If the dot collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider, wall ) )
    {
        //Move back
        mPosY -= mVelY;
				mCollider.y = mPosY;
    }

}

void Dot::render()
{
    //Show the dot
    gDotTexture->render( mPosX, mPosY );
}


