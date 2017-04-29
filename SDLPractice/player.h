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
#include "ltexture.h"
#include "particle.h"
#include "tile.h"

namespace mygame{
    
    class Player
    {
    private:
        static const int WALKING_ANIM_CNT = 4;
        static const int WALKING_ANIM_DISPFRAME = 8;
        
        static const int PLAYER_X      = 0;
        static const int PLAYER_Y      = 0;
        static const int PLAYER_VEL    = 8;
        
        static const int COLLISION_NUM = 1;
        SDL_Rect collisions[COLLISION_NUM] = {
            {8,0,16,32}};
        
        enum Dir{
            LEFT  = 0,
            RIGHT = 1,
            UP    = 2,
            DOWN  = 3,
        };
        
        int mPosX, mPosY;
        int mVelX, mVelY;
        Dir mDir;
        std::vector<SDL_Rect> mColliders;
        void shiftColliders();
        
        //SpriteData
        int mSpriteSheetIndex;
        int mSheetCharaIndex;
        int mSheetAnimIndex;
        int mR, mG, mB;
        int mA;
        int mDegrees;
        SDL_RendererFlip mFlipType;
        
        std::vector<Particle*> mParticles;
        
    public:
        static const int PLAYER_WIDTH  = 32;
        static const int PLAYER_HEIGHT = 32;
        static const int TOTAL_PARTICLES = 20;
        
        Player();
        ~Player();
        
        void handleEvent( SDL_Event& e );
        void move( int frame, std::vector<Tile*>& tiles );
        
        // アクセス関数
        int getPosX();
        int getPosY();
        void setPosX(int posX);
        void setPosY(int posY);
        std::vector<SDL_Rect>& getColliders();
        
        int getRed();
        int getGreen();
        int getBlue();
        int getAlpha();
        int getSpriteSheetIndex();
        int getSpriteAnimIndex();
        int getDir();
        std::vector<Particle*>& getParticles();
    };
}

#endif /* player_h */
