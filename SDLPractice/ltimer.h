//
//  ltimer.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef ltimer_h
#define ltimer_h

#include <SDL2/SDL.h>

namespace mygame{
    
    class LTimer
    {
    private:
        Uint32 mStartTicks;
        Uint32 mPausedTicks;
        
        bool mPaused;
        bool mStarted;
        
    public:
        LTimer();
        
        void start();
        void stop();
        void pause();
        void unpause();
        
        // アクセス関数
        Uint32 getTicks();
        bool isStarted();
        bool isPaused();
        
    };
    
}

#endif /* ltimer_h */
