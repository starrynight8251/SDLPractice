//
//  gamemanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/22.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "sdlmanager.h"
#include "gamemanager.h"
#include "graphicmanager.h"
#include "soundmanager.h"

namespace mygame{
    
    bool SDLManager::init()
    {
        bool success = true;
        
        // **** ウィンドウ、レンダラ ****
        mWindow = NULL;
        mRenderer = NULL;
        
        // **** コントローラー ****
        // コントローラ１
        mGameController = NULL;
        mControllerHaptic = NULL;
        
        // SDL初期化
        // SDL_INIT_VIDEO グラフィック
        // SDL_INIT_AUDIO サウンド
        // SDL_INIT_JOYSTICK コントローラ
        // SDL_INIT_HAPTIC コントローラ振動機能
        if( SDL_Init( SDL_INIT_VIDEO    |
                      SDL_INIT_AUDIO    |
                      SDL_INIT_JOYSTICK |
                      SDL_INIT_HAPTIC
                     ) < 0 )
        {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            mWindow = new LWindow();
            
            // ウィンドウ作成
            if( !mWindow->init() )
            {
                printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                // レンダラ作成
                mRenderer = mWindow->createRenderer();
                if( mRenderer == NULL )
                {
                    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                    success = false;
                }
                else
                {
                    // レンダラのクリア色を設定する
                    SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    
                    // JPG,PNG 読込のために SDL_image を初期化
                    int imgFlags =  IMG_INIT_JPG |
                                    IMG_INIT_PNG ;
                    if( !( IMG_Init( imgFlags ) & imgFlags ) )
                    {
                        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                        success = false;
                    }
                    
                    // TTF 読み込みのために SDL_ttf を初期化
                    if( TTF_Init() == -1 )
                    {
                        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                        success = false;
                    }
                    
                    // WAV,OGG,MP3 読込のために SDL_mixer を初期化
                    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
                    {
                        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                        success = false;
                    }
                    
                }
            }
            
            // 接続されているコントローラの数をチェックする
            // 一つもなかったら警告
            if( SDL_NumJoysticks() < 1 )
            {
                printf( "Warning: No joysticks connected!\n" );
            }
            else
            {
                // コントローラを取得
                mGameController = SDL_JoystickOpen( 0 );
                if( mGameController == NULL )
                {
                    printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
                }
                else
                {
                    // コントローラ振動機能を取得
                    mControllerHaptic = SDL_HapticOpenFromJoystick( mGameController );
                    if( mControllerHaptic == NULL )
                    {
                        printf( "Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError() );
                    }
                    else
                    {
                        // コントローラ振動機能を初期化
                        if( SDL_HapticRumbleInit( mControllerHaptic ) < 0 )
                        {
                            printf( "Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError() );
                        }
                    }
                }
            }
            
            // テクスチャフィルタリングを線形にする
            if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            {
                printf( "Warning: Linear texture filtering not enabled!" );
            }
            
        }
        
        // ゲームマネージャ初期化
        GameManager* gm_manager = &GameManager::getInstance();
        gm_manager->init();
        // グラフィックマネージャ初期化
        GraphicManager* grp_manager = &GraphicManager::getInstance();
        grp_manager->init();
        // サウンドマネージャ初期化
        SoundManager* snd_manager = &SoundManager::getInstance();
        snd_manager->init();
        
        return success;
    }
    
    bool SDLManager::loadMedia()
    {
        bool success = true;
        
        // ゲームマネージャロード処理
        GameManager* gm_manager = &GameManager::getInstance();
        if(!gm_manager->loadMedia())
        {
            printf( "Failed to GameManager loadMedia\n" );
            success = false;
        }
        // グラフィックマネージャロード処理
        GraphicManager* grp_manager = &GraphicManager::getInstance();
        if(!grp_manager->loadMedia( gm_manager->mScene ))
        {
            printf( "Failed to GraphicManager loadMedia!\n" );
            success = false;
        };
        // サウンドマネージャロード処理
        SoundManager* snd_manager = &SoundManager::getInstance();
        if(!snd_manager->loadMedia( gm_manager->mScene ))
        {
            printf( "Failed to SoundManager loadMedia!\n" );
            success = false;
        }
        
        // データ読込を完了済にする
        gm_manager->setSceneLoaded(true);
        return success;
    }
    
    // ***********************************
    //           SDLメイン処理
    // ***********************************
    void SDLManager::mainloop()
    {
        GameManager* gm_manager = &GameManager::getInstance();
        GraphicManager* grp_manager = &GraphicManager::getInstance();
        SoundManager* snd_manager = &SoundManager::getInstance();
        
        bool quit = false;
        SDL_Event e;

        // データ設定処理
        gm_manager->load();
        
        // メインイベントループ
        while( !quit )
        {
            // データ読込が完了していなければ読込
            if( !gm_manager->getSceneLoaded() ){
                // メディアデータ読込
                if(!loadMedia()){
                    printf( "Failed to loadMedia!\n" );
                    break;
                }
                // 音楽を開始
                snd_manager->musicstart();
            }
            
            // イベント判別処理
            while( SDL_PollEvent( &e ) != 0 )
            {
                //　ウィンドウを閉じる
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
                // コントローラ用イベント判別処理
                if( !mWindow->isMinimized() )
                {
                    gm_manager->handleEvent( e );
                }
                // ウィンドウ用イベント判別処理
                mWindow->handleEvent( e );
            }
            
            // ウィンドウを最小化した時
            if( mWindow->isMinimized() )
            {
                // FPS計測停止
                gm_manager->FPSStop();
                // 音楽を一時停止する
                snd_manager->musicpause();
            }
            // 通常ウィンドウの時
            else
            {
                // FPS計測再開
                gm_manager->FPSStart();
                // 音楽を再開する
                snd_manager->musicresume();
                
                // **** 状態更新処理　****
                gm_manager->changeState();
                // **** 状態反映処理 ****
                grp_manager->update();
                snd_manager->update();
                // **** フレーム進行処理 ****
                gm_manager->advanceFrame();
            }
        }
        // セーブ処理
        gm_manager->saveMedia();
    }
 
    void SDLManager::cleanup()
    {
        // **** 開放処理 ****
        // ゲーム
        GameManager* gm_manager = &GameManager::getInstance();
        gm_manager->cleanup();
        
        // グラフィック
        GraphicManager* grp_manager = &GraphicManager::getInstance();
        grp_manager->cleanup();
        
        // サウンド
        SoundManager* snd_manager = &SoundManager::getInstance();
        snd_manager->cleanup();
        
        // コントローラ
        SDL_HapticClose( mControllerHaptic );
        SDL_JoystickClose( mGameController );
        mGameController = NULL;
        mControllerHaptic = NULL;
        
        // レンダラ,ウィンドウ
        SDL_DestroyRenderer( mRenderer );
        mRenderer = NULL;
        delete mWindow;
        mWindow = NULL;
        
        // SDL_ttf, SDL_mixer, SDL_image, SDL の解放
        TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
    }
    
    SDL_Renderer* SDLManager::getRenderer()
    {
        return mRenderer;
    }
    
 }
