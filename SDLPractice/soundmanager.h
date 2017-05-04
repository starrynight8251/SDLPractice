//
//  soundmanager.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef soundmanager_h
#define soundmanager_h

#include <SDL2_mixer/SDL_mixer.h>
#include "gamemanager.h"

namespace mygame{
    
    // サウンドマネージャ
    // シングルトン、オブザーバー
    class SoundManager
    {
    private:
        Mix_Music *mMusic;
        Mix_Chunk *mScratch;
        Mix_Chunk *mHigh;
        Mix_Chunk *mMedium;
        Mix_Chunk *mLow;
        
        SoundManager(){}
        SoundManager(const SoundManager&);
        SoundManager& operator=(const SoundManager&);
        ~SoundManager(){}
        
    public:
        static SoundManager& getInstance()
        {
            static SoundManager inst;
            return inst;
        }
        
        bool init();
        bool loadMedia(GameManager::SCENE scene);
        void musicstart();
        void musicstop();
        void musicresume();
        void musicpause();
        void cleanup();
        
        void update();
        
    };
    
}

#endif /* soundmanager_h */
