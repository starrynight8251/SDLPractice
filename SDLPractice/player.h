//
//  player.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef player_h
#define player_h

#pragma execution_character_set("utf-8")

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
        
        
        enum Dir{
            LEFT  = 0,
            RIGHT = 1,
            UP    = 2,
            DOWN  = 3,
        };
        
        int mPosX, mPosY;
        int mVelX, mVelY;
        Dir mDir;
        std::vector<SDL_Rect> mCollisions;
        std::vector<SDL_Rect> mColliders;
        
        
        //SpriteData
        int mSpriteSheetIndex;
        int mSheetCharaIndex;
        int mSheetAnimIndex;
        int mR, mG, mB;
        int mA;
        int mDegrees;
        SDL_RendererFlip mFlipType;
        
        std::vector<Particle*> mParticles;
        
        // 効果音用フラグ
        bool mHitWall;
        
        // 当たり判定用四角形更新
        void shiftColliders();
        
    public:
        static const int PLAYER_WIDTH  = 32;
        static const int PLAYER_HEIGHT = 32;
        static const int TOTAL_PARTICLES = 20;
        
        Player();
        ~Player();
        
        void handleEvent( SDL_Event& e );
        void changeState( int frame );
        
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
        
        bool getHitWall();
    };
    
}

#endif /* player_h */
