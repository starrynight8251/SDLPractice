//
//  particle.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/19.
//  Copyright © 2017年 NoCompany. All rights reserved.
//
#include "particle.h"
#include "graphicmanager.h"

namespace mygame{
Particle::Particle( int x, int y )
{
    GraphicManager* grp_manager = &GraphicManager::getInstance();
    // オフセット設定
    mPosX = x + 8 + ( rand() % 12 );
    mPosY = y + ( rand() % 28 );
    
    // アニメーション初期化
    mFrame = rand() % 5;
    
    // パーティクル種類初期化
    switch( rand() % 3 )
    {
        case 0: mTexture = grp_manager->gRedTexture;break;
        case 1: mTexture = grp_manager->gGreenTexture; break;
        case 2: mTexture = grp_manager->gBlueTexture; break;
    }
}

void Particle::render(SDL_Rect& camera)
{
    GraphicManager* grp_manager = &GraphicManager::getInstance();
    // パーティクル描画
    mTexture->render( mPosX-camera.x, mPosY-camera.y);
    
    // パーティクル輝　描画
    if( mFrame % 2 == 0 )
    {
        grp_manager->gShimmerTexture->render( mPosX-camera.x, mPosY-camera.y );
    }
    
    mFrame++;
}

bool Particle::isDead()
{
    return mFrame > 10;
}
}
