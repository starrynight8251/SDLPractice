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
    private:
        SDL_Window* mWindow;
        
        int mWidth;
        int mHeight;
        
        bool mMouseFocus;
        bool mKeyboardFocus;
        bool mMinimized;
        bool mFullScreen;
        
    public:
        LWindow();
        void free();
        bool init();
        void handleEvent( SDL_Event& e );
        SDL_Renderer* createRenderer();
        
        // アクセス関数
        int getWidth();
        int getHeight();
        
        bool hasMouseFocus();
        bool hasKeyboardFocus();
        bool isMinimized();
        
    };
}
#endif /* lwindow_h */
