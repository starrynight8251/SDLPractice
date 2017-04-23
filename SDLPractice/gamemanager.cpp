//
//  gamemanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/22.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "gamemanager.h"
#include <sstream>
#include <iomanip>
#include "ltimer.h"
#include "graphicmanager.h"
#include "soundmanager.h"

namespace mygame{

bool GameManager::init()
{
    bool success = true;

    // **** ウィンドウ、レンダラ ****
    gWindow = NULL;
    gRenderer = NULL;
    
    // **** コントローラー ****
    // コントローラ１
    gGameController = NULL;
    gControllerHaptic = NULL;

    // SDL初期化
    // SDL_INIT_VIDEO グラフィック
    // SDL_INIT_AUDIO サウンド
    // SDL_INIT_JOYSTICK コントローラ
    // SDL_INIT_HAPTIC コントローラ振動機能
    if( SDL_Init( SDL_INIT_VIDEO    | SDL_INIT_AUDIO |
                 SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        gWindow = new LWindow();
        
        // ウィンドウ作成
        if( !gWindow->init() )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // レンダラ作成
            // SDL_RENDERER_ACCELERATED ハードウェア支援機能有り
            // SDL_RENDERER_PRESENTVSYNC 画面の更新タイミングを待つ(垂直同期)
            gRenderer = gWindow->createRenderer();
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                // レンダラのクリア色を設定する
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                // JPG,PNG 読込のために SDL_image を初期化
                int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG ;
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
            gGameController = SDL_JoystickOpen( 0 );
            if( gGameController == NULL )
            {
                printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                // コントローラ振動機能を取得
                gControllerHaptic = SDL_HapticOpenFromJoystick( gGameController );
                if( gControllerHaptic == NULL )
                {
                    printf( "Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError() );
                }
                else
                {
                    // コントローラ振動機能を初期化
                    if( SDL_HapticRumbleInit( gControllerHaptic ) < 0 )
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
    
    // セーブデータ用領域確保
    gData.resize( TOTAL_DATA );
    
    // グラフィックマネージャ初期化
    GraphicManager* grp_manager = &GraphicManager::getInstance();
    grp_manager->init();
    
    // サウンドマネージャ初期化
    SoundManager* snd_manager = &SoundManager::getInstance();
    snd_manager->init();

    return success;
}

bool GameManager::loadMedia()
{
    bool success = true;

    // **** セーブデータロード ****
    // バイナリファイルを読み込みモードで開く
    SDL_RWops* file = SDL_RWFromFile( "save/nums.bin", "r+b" );
    
    // ファイルが存在しなかった
    if( file == NULL )
    {
        printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );
        
        // バイナリファイルを書き込みモードで開く（新規作成）
        file = SDL_RWFromFile( "save/nums.bin", "w+b" );
        if( file != NULL )
        {
            printf( "New file created!\n" );
            
            // データ初期化
            for( int i = 0; i < TOTAL_DATA; ++i )
            {
                gData[ i ] = 0;
                SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
            }
            
            // ファイルを閉じる
            SDL_RWclose( file );
        }
        else
        {
            printf( "Error: Unable to create file! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
    }
    // ファイルが存在する
    else
    {
        // データをロードする
        printf( "Reading file...!\n" );
        for( int i = 0; i < TOTAL_DATA; ++i )
        {
            SDL_RWread( file, &gData[ i ], sizeof(Sint32), 1 );
        }
        
        // ファイルを閉じる
        SDL_RWclose( file );
        
    }
    
    // **** グラフィックロード ****
    GraphicManager* grp_manager = &GraphicManager::getInstance();
    grp_manager->loadMedia();

    // **** サウンドロード ****
    SoundManager* snd_manager = &SoundManager::getInstance();
    snd_manager->loadMedia();
    snd_manager->musicstart();
    
    // セーブデータからデータ設定
    grp_manager->setPlayerPos(gData[0], gData[1]);
    
    return success;
}
    
void GameManager::mainloop()
{
    GraphicManager* grp_manager = &GraphicManager::getInstance();
    SoundManager* snd_manager = &SoundManager::getInstance();
    
    bool quit = false;
    
    SDL_Event e;
    
    // 現在のフレーム
    int frame = 0;
    
    // FPS計測用
    LTimer fpsTimer;// タイマー
    int prev_frame = 0;// 最小化したときのフレーム
    float avgFPS = 0.0f;// FPS計算結果
    std::stringstream timeText;// 表示用テキスト
    SDL_Color textColor = { 255, 0, 255 };// テキストカラー
    
    // メインイベントループ
    while( !quit )
    {
        // イベント判別処理
        while( SDL_PollEvent( &e ) != 0 )
        {
            //　ウィンドウを閉じる
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            // コントローラ用イベント判別処理
            if( !gWindow->isMinimized() )
            {
                grp_manager->handleEvent( e );
            }
            // ウィンドウ用イベント判別処理
            gWindow->handleEvent( e );
        }
        
        // ウィンドウを最小化した時
        if( gWindow->isMinimized() )
        {
            // FPS計測を再開する
            if(fpsTimer.isStarted())
            {
                fpsTimer.stop();
            }
            // 音楽を一時停止する
            snd_manager->musicpause();
        }
        // 通常ウィンドウの時
        else
        {
            // FPS計測中かチェック
            if(!fpsTimer.isStarted())
            {
                // FPS計測が止まっていれば開始する
                prev_frame = frame;
                fpsTimer.start();
            }
            // 音楽を再開する
            snd_manager->musicresume();
            
            // **** 更新処理　****
            // プレイヤー・カメラ位置更新
            grp_manager->move(frame);
            grp_manager->setCamera();
            
            // FPS情報更新
            timeText.str( "" );
            timeText << std::fixed << std::setprecision(2) << avgFPS << "fps";
            grp_manager->setFPS(timeText,textColor);
            
            // **** 描画処理 ****
            grp_manager->render(frame);

            // 次フレーム
            ++frame;
            // 秒間フレーム数計算
            avgFPS = (frame - prev_frame) / ( fpsTimer.getTicks() / 1000.f );
        }
    }
}


void GameManager::cleanup()
{
    // **** データセーブ ****
    GraphicManager* grp_manager = &GraphicManager::getInstance();
    grp_manager->setData(gData);
    
    // バイナリファイルを書き込みモードで開く
    SDL_RWops* file = SDL_RWFromFile( "save/nums.bin", "w+b" );
    if( file != NULL )
    {
        // データをセーブする
        for( int i = 0; i < TOTAL_DATA; ++i )
        {
            SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
        }
        
        // ファイルを閉じる
        SDL_RWclose( file );
    }
    else
    {
        printf( "Error: Unable to save file! %s\n", SDL_GetError() );
    }
    
    // グラフィック
    grp_manager->cleanup();
    
    // サウンド
    SoundManager* snd_manager = &SoundManager::getInstance();
    snd_manager->cleanup();

    // コントローラ
    SDL_HapticClose( gControllerHaptic );
    SDL_JoystickClose( gGameController );
    gGameController = NULL;
    gControllerHaptic = NULL;
    
    // レンダラ,ウィンドウ
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    delete gWindow;
    
    // SDL_ttf, SDL_mixer, SDL_image, SDL の解放
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
    
}// end of namespace
