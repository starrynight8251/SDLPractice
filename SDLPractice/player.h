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

class Player
{
public:
    // 歩行アニメーション
    // 種類と表示フレーム数
	static const int WALKING_ANIM_CNT = 4;
    static const int WALKING_ANIM_DISPFRAME = 4;
 
    // 画像サイズ設定
    static const int PLAYER_X      = 0;
    static const int PLAYER_Y      = 0;
    static const int PLAYER_WIDTH  = 32;
    static const int PLAYER_HEIGHT = 32;
    
    // 当たり判定
    static const int PLAYER_COLLIDE_X =  8;
    static const int PLAYER_COLLIDE_Y =  0;
    static const int PLAYER_COLLIDE_W = 16;
    static const int PLAYER_COLLIDE_H = 32;
    
    // 移動単位
    static const int PLAYER_VEL = 8;
    
    // 向きと対応する画像位置
    enum Dir{
        LEFT  = 3,
        RIGHT = 1,
        UP    = 0,
        DOWN  = 2,
    };
 
    Player();
    ~Player(); 

    void handleEvent( SDL_Event& e );

    void move( SDL_Rect& wall );
    void render( int frame );
    
private:
	LTexture* mPlayerTexture;
	SDL_Rect  mPlayerClips[ WALKING_ANIM_CNT ];

    int mPosX, mPosY;
    Dir mDir;

    int mVelX, mVelY;
    int mR, mG, mB;
    int mA;
    
    int mDegrees;
    SDL_RendererFlip mFlipType;

    // 当たり判定
    SDL_Rect mCollider;

};


#endif /* player_h */
