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
#include <SDL2_mixer/SDL_mixer.h>
#include "lwindow.h"
#include "ltexture.h"
#include "player.h"

namespace mygame{// start of namespace
    
// ゲームマネージャ
// シングルトン
class GameManager{
private:
    GameManager(){}
    GameManager(const GameManager&);
    GameManager& operator=(const GameManager&);
    ~GameManager(){}
    bool setTiles(std::vector<Tile*>& tiles);
    
public:
    LWindow* gWindow;
    SDL_Renderer* gRenderer;
    // **** ゲームセーブ ****
    static const int TOTAL_DATA = 10;
    std::vector<Sint32> gData;

    // **** グラフィック ****
    // 背景画像サイズ
    static const int LEVEL_WIDTH = 1280;
    static const int LEVEL_HEIGHT = 960;
    // スクリーンサイズ
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;
    // タイルマップ用定数
    static const int TILE_WIDTH = 80;
    static const int TILE_HEIGHT = 80;
    static const int TOTAL_TILES = 192;
    static const int TOTAL_TILE_SPRITES = 12;
    // タイルの種別
    static const int TILE_RED = 0;
    static const int TILE_GREEN = 1;
    static const int TILE_BLUE = 2;
    static const int TILE_CENTER = 3;
    static const int TILE_TOP = 4;
    static const int TILE_TOPRIGHT = 5;
    static const int TILE_RIGHT = 6;
    static const int TILE_BOTTOMRIGHT = 7;
    static const int TILE_BOTTOM = 8;
    static const int TILE_BOTTOMLEFT = 9;
    static const int TILE_LEFT = 10;
    static const int TILE_TOPLEFT = 11;
    // パーティクル（粒子）の数
    static const int TOTAL_PARTICLES = 20;
    
    Player* gPlayer;
    LTexture* gTileTexture;
    LTexture* gTextTexture;
    LTexture* gBGTexture;
    LTexture* gRedTexture;
    LTexture* gGreenTexture;
    LTexture* gBlueTexture;
    LTexture* gShimmerTexture;
    std::vector<Tile*> gTiles;
    std::vector<SDL_Rect> gTileClips;
    TTF_Font* gFont;
    
    // **** サウンド　****
//    Mix_Music *gMusic;
//    Mix_Chunk *gScratch;
//    Mix_Chunk *gHigh;
//    Mix_Chunk *gMedium;
//    Mix_Chunk *gLow;
    
    // **** コントローラー ****
    // コントローラアナログスティックの無反応範囲
    static const int JOYSTICK_DEAD_ZONE = 8000;
    // コントローラ１
    SDL_Joystick* gGameController;
    SDL_Haptic* gControllerHaptic;
    
    static GameManager& getInstance(){
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
