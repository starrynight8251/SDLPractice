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

namespace mygame{
    
    // グラフィックマネージャ
    // シングルトン、オブザーバー
    class GraphicManager
    {
    private:
        static const int TOTAL_SPRITE_SHEETS = 1;
        static const int TOTAL_MAP_SHEETS = 1;
        
        SDL_Rect mCamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        
        std::vector<LTexture*> mBGTextures;
        std::vector<LTexture*> mMapSheetTextures;
        std::vector<LTexture*> mSpriteSheetTextures;
        std::vector<LTexture*> mParticleTextures;
        std::vector<LTexture*> mTextTextures;
        
        GraphicManager(){}
        GraphicManager(const GraphicManager&);
        GraphicManager& operator=(const GraphicManager&);
        ~GraphicManager(){}
        
    public:
        // スクリーンサイズ
        static const int SCREEN_WIDTH = 640;
        static const int SCREEN_HEIGHT = 480;
        
        TTF_Font* mFont;
        
        static GraphicManager& getInstance()
        {
            static GraphicManager inst;
            return inst;
        }
        
        bool init();
        bool loadMedia();
        void update(int frame);
        void setCamera(int px, int py, int pw, int ph);
        void setText(std::stringstream& text, SDL_Color& textColor);
        void cleanup();
        
    };
    
}

#endif /* graphicmanager_h */
