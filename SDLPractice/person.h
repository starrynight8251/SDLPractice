//
//  Person.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef person_h
#define person_h

#include <SDL2/SDL.h>
#include <vector>
#include "ltexture.h"
#include "particle.h"
#include "tile.h"

namespace mygame{
    
class Person
{
private:
    static const int WALKING_ANIM_CNT = 4;
    static const int WALKING_ANIM_DISPFRAME = 8;
    
    static const int Person_X      = 0;
    static const int Person_Y      = 0;
    
    static const int COLLISION_NUM = 1;
    SDL_Rect collisions[COLLISION_NUM] = {
        {8,0,16,32}};
    
    static const int Person_VEL = 8;
    
    enum Dir{
        LEFT  = 3,
        RIGHT = 1,
        UP    = 0,
        DOWN  = 2,
    };
    
    LTexture* mPersonTexture;
    SDL_Rect  mPersonClips[ WALKING_ANIM_CNT ];
    
    std::vector<Particle*> particles;
    void renderParticles(SDL_Rect& camera);
    
    int mPosX, mPosY;
    Dir mDir;
    
    int mVelX, mVelY;
    int mR, mG, mB;
    int mA;
    
    int mDegrees;
    SDL_RendererFlip mFlipType;
    std::vector<SDL_Rect> mColliders;
    void shiftColliders();
    
public:
    static const int Person_WIDTH  = 32;
    static const int Person_HEIGHT = 32;
    
    Person( int idx, std::vector<Tile*>& tiles );
    ~Person();
    
    void move( int frame, std::vector<Tile*>& tiles );
    void setCamera(SDL_Rect& camera);
    void render( int frame , SDL_Rect& camera );
    int getPosX();
    int getPosY();
    void setPosX(int posX);
    void setPosY(int posY);
    std::vector<SDL_Rect>& getColliders();
};
}

#endif /* person_h */
