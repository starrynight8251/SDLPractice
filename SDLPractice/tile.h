//
//  tile.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/17.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef tile_h
#define tile_h
#include <SDL2/SDL.h>

namespace mygame{
    
    class Tile
    {
    private:
        // 当たり判定用四角
        SDL_Rect mBox;
        int mType;
        
    public:
        static const int TILE_WIDTH = 80;
        static const int TILE_HEIGHT = 80;
        
        // タイルの種類
        enum Type{
            TILE_RED = 0,
            TILE_GREEN = 1,
            TILE_BLUE = 2,
            TILE_CENTER = 3,
            TILE_TOP = 4,
            TILE_TOPRIGHT = 5,
            TILE_RIGHT = 6,
            TILE_BOTTOMRIGHT = 7,
            TILE_BOTTOM = 8,
            TILE_BOTTOMLEFT = 9,
            TILE_LEFT = 10,
            TILE_TOPLEFT = 11,
            TOTAL_TILE_SPRITES = 12
        };
        
        // 位置とタイルの種類を初期化する
        Tile( int x, int y, int tileType );
        
        // アクセス関数
        SDL_Rect getBox();
        int getType();
        
    };
    
}

#endif /* tile_h */
