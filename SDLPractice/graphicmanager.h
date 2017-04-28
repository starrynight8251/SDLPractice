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
    static const int TOTAL_SPRITE_SHEETS = 1;
    static const int TOTAL_MAP_SHEETS = 1;
    
    // スクリーンサイズ
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;
    
    // パーティクル（粒子）の数
    static const int TOTAL_PARTICLES = 20;
    
    SDL_Rect mCamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    std::vector<LTexture*> mBGTextures;
    std::vector<LTexture*> mMapSheetTextures;
    std::vector<LTexture*> mSpriteSheetTextures;
    std::vector<LTexture*> mParticleTextures;
    std::vector<LTexture*> mTextTextures;

    std::vector<SDL_Rect> mTileClips;
    TTF_Font* mFont;
    
    static GraphicManager& getInstance()
    {
        static GraphicManager inst;
        return inst;
    }
    
    bool init();
    bool loadMedia();
    void render(int frame);
    void setCamera(int px, int py, int pw, int ph);
    void setFPS(std::stringstream& timeText, SDL_Color& textColor);
    void cleanup();
    
};
}// end of namaspace

#endif /* graphicmanager_h */
