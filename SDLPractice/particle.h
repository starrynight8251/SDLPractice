//
//  particle.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/19.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef particle_h
#define particle_h

class Particle
{
public:
    Particle( int x, int y );
    void render(SDL_Rect& camera);
    bool isDead();
    
private:
    int mPosX, mPosY;
    int mFrame;
    LTexture *mTexture;
};

#endif /* particle_h */
