//
//  lwindow.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/12.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef lwindow_h
#define lwindow_h

#include <SDL2/SDL.h>
namespace mygame{
class LWindow
{
public:
    LWindow();
 
    bool init();

    SDL_Renderer* createRenderer();

    void handleEvent( SDL_Event& e );
    void free();

    int getWidth();
    int getHeight();

    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    
private:
    SDL_Window* mWindow;
 
    int mWidth;
    int mHeight;

    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mMinimized;
    bool mFullScreen;

};
}
#endif /* lwindow_h */
