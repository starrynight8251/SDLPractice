//
//  gamemanager.h
//  SDLPractice
//
//  Created by starrynight on 2017/05/04.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef gamemanager_h
#define gamemanager_h

#pragma execution_character_set("utf-8")

#include <SDL2/SDL.h>
#include <sstream>
#include <vector>
#include "ltimer.h"
#include "player.h"

namespace mygame{
    
    // ゲームマネージャ
    // シングルトン
    class GameManager
    {
    private:
        // **** ゲームセーブ ****
        // セーブデータ一時保存領域
        static const int TOTAL_DATA = 10;
        
        // ゲーム用データ
        std::vector<Sint32> mData;
        std::vector<Tile*> mMap;
        Player* mPlayer;
        std::stringstream mFPSText;
        std::string mBasePath;// カレントディレクトリ取得用
        
        GameManager(){}
        GameManager(const GameManager&);
        GameManager& operator=(const GameManager&);
        ~GameManager(){}
        
    public:
        // **** ゲームマップ ****
        static const int MAP_WIDTH = 1280;
        static const int MAP_HEIGHT = 960;
        static const int TOTAL_TILES = 192;
        
        std::string BASE_PATH;//実行ファイルのあるパス
        // 各ファイルのパス（BASE_PATHからの相対パス）
        std::string GRP_PATH;
        std::string SND_PATH;
        std::string FNT_PATH;
        std::string SAV_PATH;
        
        int mFrame;
        // FPS計測用
        LTimer mFPSTimer;// タイマー
        int mPrevFrame;// 最小化したときのフレーム
        float mAvgFPS;// FPS計算結果
        
        // **** ゲームシーン ****
        enum SCENE {
            INIT,
            LEVEL1,
            LEVEL_,
        };
        
        static GameManager& getInstance()
        {
            static GameManager inst;
            return inst;
        }
        
        bool init();
        
        bool loadMedia();
        bool loadData();
        bool loadMap();
        void load();
        
        void handleEvent(SDL_Event& e);
        void FPSStart();
        void FPSStop();
        void changeState();
        void advanceFrame();
        
        void saveMedia();
        bool saveData();
        
        void cleanup();
        
        SCENE mScene;
        bool mSceneLoaded;
        
        // アクセス関数
        std::vector<Tile*>& getMap();
        Player* getPlayer();
        std::stringstream& getFPSText();
        void setSceneLoaded(bool loaded);
        bool getSceneLoaded();
    };
    
}

#endif /* gamemanager_h */
