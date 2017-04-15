//
//  ltimer.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef ltimer_h
#define ltimer_h

class LTimer
{
public:
    LTimer();

    void start();
    void stop();
    void pause();
    void unpause();

    Uint32 getTicks();

    bool isStarted();
    bool isPaused();
    
private:
    Uint32 mStartTicks;
    Uint32 mPausedTicks;

    bool mPaused;
    bool mStarted;
};


#endif /* ltimer_h */
