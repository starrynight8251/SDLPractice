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

class Player
{
public:
    static const int PLAYER_WIDTH  = 32;
    static const int PLAYER_HEIGHT = 32;

    Player();
    ~Player();
    
    void handleEvent( SDL_Event& e );
    void move( std::vector<SDL_Rect>& otherColliders );
    void render( int frame , int x, int y );
    int getPosX();
    int getPosY();
    std::vector<SDL_Rect>& getColliders();

private:
    // 歩行アニメーション
    // 種類と表示フレーム数
	static const int WALKING_ANIM_CNT = 4;
    static const int WALKING_ANIM_DISPFRAME = 4;
 
    // 画像サイズ設定
    static const int PLAYER_X      = 304;
    static const int PLAYER_Y      = 224;
    
    // 当たり判定用四角形（通常は１つだけ、拡張可能）
    static const int COLLISION_NUM = 1;
    SDL_Rect collisions[COLLISION_NUM] = {
        {8,0,16,32}};
    
    // 移動単位
    static const int PLAYER_VEL = 8;
    
    // 向きと対応する画像位置
    enum Dir{
        LEFT  = 3,
        RIGHT = 1,
        UP    = 0,
        DOWN  = 2,
    };

	LTexture* mPlayerTexture;
	SDL_Rect  mPlayerClips[ WALKING_ANIM_CNT ];

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


#endif /* player_h */
