//
//  particle.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/19.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef particle_h
#define particle_h

#pragma execution_character_set("utf-8")

#include <SDL2/SDL.h>
#include "ltexture.h"

namespace mygame{
    
    class Particle
    {
    private:
        int mPosX, mPosY;
        int mLife;
        int mTexIndex;
        
    public:
        Particle( int x, int y );
        void decLife();
        
        // アクセス関数
        bool isDead();
        int getPosX();
        int getPosY();
        int getLife();
        int getTexIndex();
    };
    
}

#endif /* particle_h */
