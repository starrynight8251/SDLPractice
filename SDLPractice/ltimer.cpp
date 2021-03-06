//
//  ltimer.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "ltimer.h"

namespace mygame{
    
    LTimer::LTimer()
    {
        mStartTicks = 0;
        mPausedTicks = 0;
        
        mPaused = false;
        mStarted = false;
    }
    
    void LTimer::start()
    {
        mStarted = true;
        mPaused = false;
        
        mStartTicks = SDL_GetTicks();
        mPausedTicks = 0;
    }
    
    void LTimer::stop()
    {
        mStarted = false;
        mPaused = false;
        
        mStartTicks = 0;
        mPausedTicks = 0;
    }
    
    void LTimer::pause()
    {
        if( mStarted && !mPaused )
        {
            mPaused = true;
            
            mPausedTicks = SDL_GetTicks() - mStartTicks;
            mStartTicks = 0;
        }
    }
    
    void LTimer::unpause()
    {
        if( mStarted && mPaused )
        {
            mPaused = false;
            
            mStartTicks = SDL_GetTicks() - mPausedTicks;
            mPausedTicks = 0;
        }
    }
    
    Uint32 LTimer::getTicks()
    {
        Uint32 time = 0;
        
        if( mStarted )
        {
            if( mPaused )
            {
                time = mPausedTicks;
            }
            else
            {
                time = SDL_GetTicks() - mStartTicks;
            }
        }
        
        return time;
    }
    
    bool LTimer::isStarted()
    {
        return mStarted;
    }
    
    bool LTimer::isPaused()
    {
        return mPaused && mStarted;
    }
    
}
