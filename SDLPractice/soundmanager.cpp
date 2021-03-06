//
//  soundmanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "soundmanager.h"
#include "gamemanager.h"

namespace mygame{
    
    bool SoundManager::init()
    {
        mMusic = NULL;
        mScratch = NULL;
        mHigh = NULL;
        mMedium = NULL;
        mLow = NULL;
        return true;
    }
    
    bool SoundManager::loadMedia( GameManager::SCENE scene )
    {
        bool success = true;
        GameManager* gm_manager = &GameManager::getInstance();
        std::string base_path = gm_manager->BASE_PATH;
        std::string snd_path  = gm_manager->SND_PATH;
        std::string path;
        
        if( scene == GameManager::SCENE::INIT ){
            path = base_path + snd_path + "scratch.wav";
            mScratch = Mix_LoadWAV( path.c_str() );
            if( mScratch == NULL )
            {
                printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                success = false;
            }
            
            path = base_path + snd_path + "high.wav";
            mHigh = Mix_LoadWAV( path.c_str() );
            if( mHigh == NULL )
            {
                printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                success = false;
            }
            
            path = base_path + snd_path + "medium.wav";
            mMedium = Mix_LoadWAV( path.c_str() );
            if( mMedium == NULL )
            {
                printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                success = false;
            }
            
            path = base_path + snd_path + "low.wav";
            mLow = Mix_LoadWAV( path.c_str() );
            if( mLow == NULL )
            {
                printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                success = false;
            }
        }
        else
        {
            if(mMusic != NULL){
                musicpause();
                musicstop();
                Mix_FreeMusic( mMusic );
            }
            
            path = base_path + snd_path + ("mario" + std::to_string(gm_manager->mScene) + ".mp3");
            mMusic = Mix_LoadMUS( path.c_str() );
            if( mMusic == NULL )
            {
                printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
                success = false;
            }
        }
        return success;
    }
    
    void SoundManager::musicstart()
    {
        // 再生中かチェック
        if( Mix_PlayingMusic() == 0 )
        {
            // 音楽を再生
            Mix_PlayMusic( mMusic, -1 );
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
    
    // ***********************************
    //          サウンドメイン処理
    // ***********************************
    void SoundManager::update()
    {
        GameManager* gm_manager = &GameManager::getInstance();
        Player* plyer = gm_manager->getPlayer();
        
        if(plyer->getHitWall())
        {
            if(Mix_Playing(1) != 1){
                Mix_PlayChannel( 1, mLow, 0);
            }
        }
    }
    
    void SoundManager::cleanup()
    {
        musicpause();
        musicstop();
        Mix_FreeChunk( mScratch );
        Mix_FreeChunk( mHigh );
        Mix_FreeChunk( mMedium );
        Mix_FreeChunk( mLow );
        Mix_FreeMusic( mMusic );
        mScratch = NULL;
        mHigh = NULL;
        mMedium = NULL;
        mLow = NULL;
        mMusic = NULL;
    }
    
}
