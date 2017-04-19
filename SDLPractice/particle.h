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
    //Initialize position and animation
    Particle( int x, int y );
    
    //Shows the particle
    void render(SDL_Rect& camera);
    
    //Checks if particle is dead
    bool isDead();
    
private:
    //Offsets
    int mPosX, mPosY;
    
    //Current frame of animation
    int mFrame;
    
    //Type of particle
    LTexture *mTexture;
};

#endif /* particle_h */
