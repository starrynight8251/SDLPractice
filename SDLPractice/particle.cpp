//
//  particle.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/19.
//  Copyright © 2017年 NoCompany. All rights reserved.
//
#include <SDL2/SDL.h>
#include <stdio.h>
#include "ltexture.h"
#include "particle.h"

extern LTexture* gRedTexture;
extern LTexture* gGreenTexture;
extern LTexture* gBlueTexture;
extern LTexture* gShimmerTexture;

Particle::Particle( int x, int y )
{
    //Set offsets
    mPosX = x + 8 + ( rand() % 12 );
    mPosY = y + ( rand() % 28 );
    
    //Initialize animation
    mFrame = rand() % 5;
    
    //Set type
    switch( rand() % 3 )
    {
        case 0: mTexture = gRedTexture;break;
        case 1: mTexture = gGreenTexture; break;
        case 2: mTexture = gBlueTexture; break;
    }
}

void Particle::render(SDL_Rect& camera)
{
    //Show image
    mTexture->render( mPosX-camera.x, mPosY-camera.y);
    
    //Show shimmer
    if( mFrame % 2 == 0 )
    {
        gShimmerTexture->render( mPosX-camera.x, mPosY-camera.y );
    }
    
    //Animate
    mFrame++;
}

bool Particle::isDead()
{
    return mFrame > 10;
}
