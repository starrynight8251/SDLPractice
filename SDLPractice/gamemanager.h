//
//  gamemanager.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/22.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef gamemanager_h
#define gamemanager_h

#include <SDL2/SDL.h>
#include <vector>
#include "lwindow.h"

namespace mygame{// start of namespace
    
// ゲームマネージャ
// シングルトン
class GameManager
{
private:
    GameManager(){}
    GameManager(const GameManager&);
    GameManager& operator=(const GameManager&);
    ~GameManager(){}
    
public:
    LWindow* gWindow;
    SDL_Renderer* gRenderer;
    // **** ゲームセーブ ****
    static const int TOTAL_DATA = 10;
    std::vector<Sint32> gData;
    
    // **** コントローラー ****
    // コントローラアナログスティックの無反応範囲
    static const int JOYSTICK_DEAD_ZONE = 8000;
    // コントローラ１
    SDL_Joystick* gGameController;
    SDL_Haptic* gControllerHaptic;
    
    static GameManager& getInstance()
    {
        static GameManager inst;
        return inst;
    }
    
    bool init();
    bool loadMedia();
    void mainloop();
    void cleanup();
    
};
    
}// end of namaspace

#endif /* gamemanager_h */
