//
//  lbutton.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/01.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef lbutton_h
#define lbutton_h

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

//The mouse button
class LButton
{
public:
    //Initializes internal variables
    LButton();
    
    //Sets top left position
    void setPosition( int x, int y );
    
    //Handles mouse event
    void handleEvent( SDL_Event* e );
    
    //Shows button sprite
    void render();
    
private:
    //Top left position
    SDL_Point mPosition;
    
    //Currently used global sprite
    LButtonSprite mCurrentSprite;
};

#endif /* lbutton_h */
