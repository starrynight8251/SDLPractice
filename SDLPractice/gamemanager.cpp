//
//  gamemanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/22.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "gamemanager.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include "ltimer.h"
#include "graphicmanager.h"
#include "soundmanager.h"

namespace mygame{
    
    bool GameManager::init()
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
        
        
        
        // ゲームデータ初期化
        mData.resize( TOTAL_DATA );// セーブデータ用
        mMap.resize( TOTAL_TILES );
        mPlayer = new Player();
        mFPSText.str( "" );
        
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
        
        // **** データロード ****
        if( !dataLoad() )
        {
            printf( "Failed to dataLoad!\n" );
            success = false;
        }
        
        // **** マップロード ****
        if( !loadMap( mMap ) )
        {
            printf( "Failed to loadMap!\n" );
            success = false;
        }
        
        // **** データ読込 ****
        mPlayer->setPosX(mData[0]);
        mPlayer->setPosY(mData[1]);
        
        // **** グラフィックロード ****
        GraphicManager* grp_manager = &GraphicManager::getInstance();
        success = grp_manager->loadMedia();
        
        // **** サウンドロード ****
        SoundManager* snd_manager = &SoundManager::getInstance();
        success = snd_manager->loadMedia();
        snd_manager->musicstart();
        
        return success;
    }
    
    // ***********************************
    //           ゲームメイン処理
    // ***********************************
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
                if( !mWindow->isMinimized() )
                {
                    mPlayer->handleEvent( e );
                }
                // ウィンドウ用イベント判別処理
                mWindow->handleEvent( e );
            }
            
            // ウィンドウを最小化した時
            if( mWindow->isMinimized() )
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
                
                // **** 状態更新処理　****
                // プレイヤー状態
                mPlayer->changeState(frame);
                // FPS計測状態
                mFPSText.str( "" );
                mFPSText << std::fixed << std::setprecision(2) << avgFPS << "fps";
                
                // **** 状態反映処理 ****
                grp_manager->update(frame);
                snd_manager->update(frame);
                
                // 次フレーム
                ++frame;
                // 秒間フレーム数計算
                avgFPS = (frame - prev_frame) / ( fpsTimer.getTicks() / 1000.f );
            }
        }
    }
    
    bool GameManager::dataLoad()
    {
        bool success = true;
        
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
                    mData[ i ] = 0;
                    SDL_RWwrite( file, &mData[ i ], sizeof(Sint32), 1 );
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
                SDL_RWread( file, &mData[ i ], sizeof(Sint32), 1 );
            }
            
            // ファイルを閉じる
            SDL_RWclose( file );
        }
        return success;
    }
    
    bool GameManager::loadMap(std::vector<Tile*>& map)
    {
        bool mapLoaded = true;
        
        // タイルのオフセット
        int x = 0, y = 0;
        
        // マップ用ファイルを開く
        std::ifstream map_file( "graphics/lazy2.map" );
        if( !map_file )
        {
            printf( "Unable to load map file!\n" );
            mapLoaded = false;
        }
        else
        {
            // タイルを初期化
            for( int i = 0; i < TOTAL_TILES; ++i )
            {
                int tileType = -1;
                
                // マップ用ファイルからタイル情報を読込
                map_file >> tileType;
                
                // 読込中に問題発生
                if( map_file.fail() )
                {
                    // 読込を停止する
                    printf( "Error loading map: Unexpected end of file!\n" );
                    mapLoaded = false;
                    break;
                }
                
                // タイル番号が適正なら
                if( ( tileType >= 0 ) && ( tileType < Tile::TOTAL_TILE_SPRITES ) )
                {
                    map[ i ] = new Tile( x, y, tileType );
                }
                // タイル番号が不適正なら
                else
                {
                    // 読込を停止する
                    printf( "Error loading map: Invalid tile type at %d!\n", i );
                    mapLoaded = false;
                    break;
                }
                
                // 次のタイルへ
                x += Tile::TILE_WIDTH;
                
                // 右端まで行ったら
                if( x >= MAP_WIDTH )
                {
                    // 左端に戻る
                    x = 0;
                    
                    // 次の列へ
                    y += Tile::TILE_HEIGHT;
                }
            }
        }
        
        // ファイルを閉じる
        map_file.close();
        
        return mapLoaded;
    }
    
    void GameManager::cleanup()
    {
        // データ書込
        mData[0] = mPlayer->getPosX();
        mData[1] = mPlayer->getPosY();
        for(int i=2; i<mData.size(); ++i){
            mData[i] = 0;
        }
        
        // **** データセーブ ****
        if(!dataSave()){
            printf( "Error: Unable to dataSave! %s\n", SDL_GetError() );
        }
        mData.clear();
        
        // **** 開放処理 ****
        // ゲームデータ
        if(mPlayer != NULL)
        {
            delete mPlayer;
            mPlayer = NULL;
        }
        for( int i = 0; i < TOTAL_TILES; ++i )
        {
            if( mMap[ i ] != NULL )
            {
                delete mMap[ i ];
                mMap[ i ] = NULL;
            }
        }
        mMap.clear();
        
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
    
    bool GameManager::dataSave()
    {
        bool success = true;
        // バイナリファイルを書き込みモードで開く
        SDL_RWops* file = SDL_RWFromFile( "save/nums.bin", "w+b" );
        if( file != NULL )
        {
            // データをセーブする
            for( int i = 0; i < TOTAL_DATA; ++i )
            {
                SDL_RWwrite( file, &mData[ i ], sizeof(Sint32), 1 );
            }
            
            // ファイルを閉じる
            SDL_RWclose( file );
        }
        else
        {
            success = false;
        }
        return success;
    }
    
    SDL_Renderer* GameManager::getRenderer()
    {
        return mRenderer;
    }
    
    std::vector<Tile*>& GameManager::getMap()
    {
        return mMap;
    }
    
    Player* GameManager::getPlayer()
    {
        return mPlayer;
    }
    
    std::stringstream& GameManager::getFPSText()
    {
        return mFPSText;
    }
    
}
