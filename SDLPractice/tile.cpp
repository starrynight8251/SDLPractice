//
//  tile.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/17.
//  Copyright © 2017年 NoCompany. All rights reserved.
//
#include <SDL2/SDL.h>
#include <vector>
#include "tile.h"
#include "ltexture.h"
#include "helper.h"

extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;
extern LTexture* gTileTexture;
extern std::vector<SDL_Rect> gTileClips;


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

void Tile::render( SDL_Rect& camera )
{
    // タイルがスクリーン上にあるなら（クリッピング）
    if( checkCollision( camera, mBox ) )
    {
        // タイル描画
        gTileTexture->render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
    }
}

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

