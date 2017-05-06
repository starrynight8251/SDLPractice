//
//  particle.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/19.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "particle.h"
#include "graphicmanager.h"

namespace mygame{
    
    Particle::Particle( int x, int y )
    {
        // オフセット設定
        mPosX = x + 8 + ( rand() % 12 );
        mPosY = y + ( rand() % 28 );
        
        // アニメーション初期化
        mLife = 15 + rand() % 5;
        
        // パーティクル種類初期化
        mTexIndex = rand() % 3;
    }
    
    void Particle::decLife()
    {
        mLife--;
    }
    
    bool Particle::isDead()
    {
        return mLife <= 0;
    }
    
    int Particle::getPosX()
    {
        return mPosX;
    }
    int Particle::getPosY()
    {
        return mPosY;
    }
    
    int Particle::getLife()
    {
        return mLife;
    }
    
    int Particle::getTexIndex()
    {
        return mTexIndex;
    }
    
}
