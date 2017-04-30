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
#include <sstream>
#include <vector>
#include "lwindow.h"
#include "player.h"

namespace mygame{
    
    // ゲームマネージャ
    // シングルトン
    class GameManager
    {
    private:
        // **** ゲームセーブ ****
        // セーブデータ受け渡し用領域
        static const int TOTAL_DATA = 10;
        
        GameManager(){}
        GameManager(const GameManager&);
        GameManager& operator=(const GameManager&);
        ~GameManager(){}
        
        LWindow* mWindow;
        SDL_Renderer* mRenderer;
        
        // コントローラ
        SDL_Joystick* mGameController;
        SDL_Haptic* mControllerHaptic;
        
        // ゲーム用データ
        std::vector<Sint32> mData;
        std::vector<Tile*> mMap;
        Player* mPlayer;
        std::stringstream mFPSText;
        std::string mBasePath;// カレントディレクトリ取得用
        
    public:
        // **** コントローラー ****
        // コントローラアナログスティックの無反応範囲
        static const int JOYSTICK_DEAD_ZONE = 8000;

        // **** ゲームマップ ****
        static const int MAP_WIDTH = 1280;
        static const int MAP_HEIGHT = 960;
        static const int TOTAL_TILES = 192;
        
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
        
        // アクセス関数
        SDL_Renderer* getRenderer();
        std::vector<Tile*>& getMap();
        Player* getPlayer();
        std::stringstream& getFPSText();
        char* getBasePath();
    };
    
}

#endif /* gamemanager_h */
