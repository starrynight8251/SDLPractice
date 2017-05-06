//
//  sdlmanager.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/22.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef sdlmanager_h
#define sdlmanager_h

#pragma execution_character_set("utf-8")

#include <SDL2/SDL.h>
#include <sstream>
#include <vector>
#include "lwindow.h"
#include "player.h"

namespace mygame{
    
    // SDLマネージャ
    // シングルトン
    class SDLManager
    {
    private:
        SDLManager(){}
        SDLManager(const SDLManager&);
        SDLManager& operator=(const SDLManager&);
        ~SDLManager(){}
        
        LWindow* mWindow;
        SDL_Renderer* mRenderer;
        
        // コントローラ
        SDL_Joystick* mGameController;
        SDL_Haptic* mControllerHaptic;

    public:
        // **** コントローラー ****
        // コントローラアナログスティックの無反応範囲
        static const int JOYSTICK_DEAD_ZONE = 8000;
        
        static SDLManager& getInstance()
        {
            static SDLManager inst;
            return inst;
        }
        
        bool init();
        bool loadMedia();
        void mainloop();
        void cleanup();
        SDL_Renderer* getRenderer();
    };
    
}

#endif /* sdlmanager_h */
