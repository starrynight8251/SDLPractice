//
//  player.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef player_h
#define player_h

#include <SDL2/SDL.h>
#include <vector>
#include "tile.h"
#include "particle.h"

namespace mygame{

class Player
{
public:
    static const int PLAYER_WIDTH  = 32;
    static const int PLAYER_HEIGHT = 32;

    Player();
    ~Player();
    
    void handleEvent( SDL_Event& e );
    void move( std::vector<Tile*>& tiles );
    void setCamera(SDL_Rect& camera);
    void render( int frame , SDL_Rect& camera );
    int getPosX();
    int getPosY();
    void setPosX(int posX);
    void setPosY(int posY);
    std::vector<SDL_Rect>& getColliders();

private:
	static const int WALKING_ANIM_CNT = 4;
    static const int WALKING_ANIM_DISPFRAME = 8;
 
    static const int PLAYER_X      = 0;
    static const int PLAYER_Y      = 0;
    
    static const int COLLISION_NUM = 1;
    SDL_Rect collisions[COLLISION_NUM] = {
        {8,0,16,32}};
 
    static const int PLAYER_VEL = 8;

    enum Dir{
        LEFT  = 3,
        RIGHT = 1,
        UP    = 0,
        DOWN  = 2,
    };

	LTexture* mPlayerTexture;
	SDL_Rect  mPlayerClips[ WALKING_ANIM_CNT ];

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

};
}

#endif /* player_h */
