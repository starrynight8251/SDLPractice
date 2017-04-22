//
//  helper.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/16.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef helper_h
#define helper_h

#include <SDL2/SDL.h>
#include <vector>
#include "tile.h"

namespace mygame{
extern bool touchesWall( std::vector<SDL_Rect>& boxes, std::vector<Tile*>& tiles );
extern bool checkCollision( SDL_Rect a, SDL_Rect b );
extern bool checkCollision( std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b );
}

#endif /* helper_h */
