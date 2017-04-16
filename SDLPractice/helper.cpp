//
//  helper.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/16.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "helper.h"

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    // 四角形の各辺
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    // 四角形aの各辺の座標を計算する
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    
    // 四角形bの各辺の座標を計算する
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    
    // Aの辺のいずれかがBの外側なら重なっていない
    if( bottomA <= topB )
    {
        return false;
    }
    
    if( topA >= bottomB )
    {
        return false;
    }
    
    if( rightA <= leftB )
    {
        return false;
    }
    
    if( leftA >= rightB )
    {
        return false;
    }
    
    // 重なっている
    return true;
}

bool checkCollision( std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b )
{
    // 四角形の各辺
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    // A側の全四角形をチェック
    for( int Abox = 0; Abox < a.size(); Abox++ )
    {
        // 四角形Aの各辺の座標を計算する
        leftA = a[ Abox ].x;
        rightA = a[ Abox ].x + a[ Abox ].w;
        topA = a[ Abox ].y;
        bottomA = a[ Abox ].y + a[ Abox ].h;
        
        // B側の全四角形をチェック
        for( int Bbox = 0; Bbox < b.size(); Bbox++ )
        {
            //　四角形Bの各辺の座標を計算する
            leftB = b[ Bbox ].x;
            rightB = b[ Bbox ].x + b[ Bbox ].w;
            topB = b[ Bbox ].y;
            bottomB = b[ Bbox ].y + b[ Bbox ].h;
            
            // Aの辺のいずれもBの内側なら重なっている
            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
            {
                return true;
            }
        }
    }
    
    // 重なっていない
    return false;
}

