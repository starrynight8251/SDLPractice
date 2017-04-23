//
//  graphicmanager.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef graphicmanager_h
#define graphicmanager_h
#include <SDL2/SDL.h>
#include "ltexture.h"
#include "player.h"

namespace mygame{// start of namespace
    
// グラフィックマネージャ
// シングルトン
class GraphicManager
{
private:
    GraphicManager(){}
    GraphicManager(const GraphicManager&);
    GraphicManager& operator=(const GraphicManager&);
    ~GraphicManager(){}
    
public:
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
    
    SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    
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

    static GraphicManager& getInstance()
    {
        static GraphicManager inst;
        return inst;
    }
    
    bool init();
    bool loadMedia();
    void setPlayerPos(int x, int y);
    bool setTiles(std::vector<Tile*>& tiles);
    void render(int frame);
    void handleEvent(SDL_Event e);
    void move();
    void setCamera();
    void setFPS(std::stringstream& timeText, SDL_Color& textColor);
    void setData(std::vector<int>& data);

    void cleanup();
    
};
}// end of namaspace

#endif /* graphicmanager_h */
