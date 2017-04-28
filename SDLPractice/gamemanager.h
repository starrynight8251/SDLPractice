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
#include "player.h"

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
    LWindow* mWindow;
    SDL_Renderer* mRenderer;

    // **** コントローラー ****
    // コントローラアナログスティックの無反応範囲
    static const int JOYSTICK_DEAD_ZONE = 8000;
    // コントローラ１
    SDL_Joystick* mGameController;
    SDL_Haptic* mControllerHaptic;
    
    // **** ゲームセーブ ****
    static const int TOTAL_DATA = 10;
    std::vector<Sint32> mData;
    
    // **** ゲームマップ ****
    std::vector<Tile*> mMap;
    static const int MAP_WIDTH = 1280;
    static const int MAP_HEIGHT = 960;
    static const int TOTAL_TILES = 192;

    // **** プレーヤー ****
    Player* mPlayer;
    
    
    static GameManager& getInstance()
    {
        static GameManager inst;
        return inst;
    }
    
    bool init();
    bool loadMedia();
    void mainloop();
    bool dataLoad();
    bool dataSave();
    bool loadMap(std::vector<Tile*>& map);
    void cleanup();
    
};
    
}// end of namaspace

#endif /* gamemanager_h */
