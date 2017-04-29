//
//  tile.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/17.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "tile.h"
#include "gamemanager.h"
#include "graphicmanager.h"
#include "helper.h"

namespace mygame{
    Tile::Tile( int x, int y, int tileType )
    {
        // オフセット位置
        mBox.x = x;
        mBox.y = y;
        
        // 当たり判定用四角
        mBox.w = TILE_WIDTH;
        mBox.h = TILE_HEIGHT;
        
        // タイルの種類を設定
        mType = tileType;
    }
    
    int Tile::getType()
    {
        return mType;
    }
    
    SDL_Rect Tile::getBox()
    {
        return mBox;
    }
}
