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
public:
    // 位置とタイルの種類を初期化する
    Tile( int x, int y, int tileType );
    
    // タイルを描画
    void render( SDL_Rect& camera );
    
    // タイルの種類を取得
    int getType();
    
    // 当たり判定用四角を取得
    SDL_Rect getBox();
    
private:
    // 当たり判定用四角
    SDL_Rect mBox;
    
    // タイルの種類
    int mType;
};
}

#endif /* tile_h */
