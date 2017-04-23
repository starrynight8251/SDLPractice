//
//  soundmanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "soundmanager.h"

namespace mygame{// start of namespace
    
bool SoundManager::init()
{
    gMusic = NULL;
    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;
    return true;
}

bool SoundManager::loadMedia()
{
    bool success = true;
    
    gMusic = Mix_LoadMUS( "sounds/mario.mp3" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gScratch = Mix_LoadWAV( "sounds/scratch.wav" );
    if( gScratch == NULL )
    {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gHigh = Mix_LoadWAV( "sounds/high.wav" );
    if( gHigh == NULL )
    {
        printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gMedium = Mix_LoadWAV( "sounds/medium.wav" );
    if( gMedium == NULL )
    {
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gLow = Mix_LoadWAV( "sounds/low.wav" );
    if( gLow == NULL )
    {
        printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    return success;
}
    
void SoundManager::musicstart()
{
    // 再生中かチェック
    if( Mix_PlayingMusic() == 0 )
    {
        // 音楽を再生
        Mix_PlayMusic( gMusic, -1 );
    }
}

void SoundManager::musicstop()
{
    // 再生中かチェック
    if( Mix_PlayingMusic() != 0 )
    {
        // 音楽を停止
        Mix_HaltMusic();
    }
}
    
void SoundManager::musicpause()
{
    // 再生中かチェック
    if( Mix_PlayingMusic() != 0 )
    {
        // 一時停止していなければ一時停止する
        if( Mix_PausedMusic() != 1 )
        {
            Mix_PauseMusic();
        }
    }
}
    
void SoundManager::musicresume()
{
    // 再生中かチェック
    if( Mix_PlayingMusic() != 0 )
    {
        // 一時停止していれば再開する
        if( Mix_PausedMusic() == 1 )
        {
            Mix_ResumeMusic();
        }
    }
}

void SoundManager::cleanup()
{
    musicpause();
    musicstop();
    Mix_FreeChunk( gScratch );
    Mix_FreeChunk( gHigh );
    Mix_FreeChunk( gMedium );
    Mix_FreeChunk( gLow );
    Mix_FreeMusic( gMusic );
    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;
    gMusic = NULL;
}

}// end of namespace
