//
//  gamemanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/05/04.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "gamemanager.h"
#include <fstream>
#include <iomanip>

namespace mygame{
    bool GameManager::init()
    {
        bool success = true;
        
        mFrame = 0;
        
        // FPS計測用
        mPrevFrame = 0;// 最小化したときのフレーム
        mAvgFPS = 0.0f;// FPS計算結果
        mFPSText.str( "" );// FPS表示用テキスト
        
        // 実行ファイルのあるパスを記録
        mBasePath = SDL_GetBasePath();
        // セーブデータ一時保存領域
        mData.resize( TOTAL_DATA );
        // マップデータ
        mMap.resize( TOTAL_TILES );
        // プレイヤーデータ
        mPlayer = new Player();
        // 現在のシーン
        mScene = SCENE::INIT;
        // シーン読込フラグ
        mSceneLoaded = false;
        
        return success;
    }
    
    bool GameManager::loadMedia()
    {
        bool success = true;
        
        // セーブデータをファイルから一時保存領域へ
        if( !loadData() )
        {
            printf( "Failed to dataLoad!\n" );
            success = false;
        }
        // マップデータ読込
        if( !loadMap() )
        {
            printf( "Failed to loadMap!\n" );
            success = false;
        }
        
        return success;
    }
    
    void GameManager::load()
    {
        // セーブデータを一時保存領域からゲームデータへ
        mPlayer->setPosX(mData[0]);
        mPlayer->setPosY(mData[1]);
        mScene = (SCENE)(mData[2] + 1);
        mSceneLoaded = false;
    }
    
    bool GameManager::loadData()
    {
        bool success = true;
        if( mScene == SCENE::INIT )
        {
            std::string path = mBasePath;// 実行ファイルディレクトリ取得
            path += "save/nums.dat";
            
            // バイナリファイルを読み込みモードで開く
            SDL_RWops* file = SDL_RWFromFile( path.c_str(), "r+b" );
            
            // ファイルが存在しなかった
            if( file == NULL )
            {
                printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );
                
                // バイナリファイルを書き込みモードで開く（新規作成）
                file = SDL_RWFromFile( path.c_str(), "w+b" );
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
        }
        return success;
    }
    
    
    bool GameManager::loadMap()
    {
        bool mapLoaded = true;
        if( mScene != SCENE::INIT ){
            // タイルのオフセット
            int x = 0, y = 0;
            
            std::string path = mBasePath;// 実行ファイルディレクトリ取得
            path += ("graphics/lazy" + std::to_string(mScene) + ".map");
            
            // マップ用ファイルを開く
            std::ifstream map_file( path.c_str() );
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
                        if( mMap[ i ] != NULL )
                        {
                            delete mMap[ i ];
                        }
                        mMap[ i ] = new Tile( x, y, tileType );
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
                // ファイルを閉じる
                map_file.close();
            }
        }
        return mapLoaded;
    }
    
    // ***********************************
    //           ゲームメイン処理
    // ***********************************
    void GameManager::handleEvent(SDL_Event& e)
    {
        mPlayer->handleEvent(e);
    }
    
    void GameManager::FPSStart()
    {
        if(!mFPSTimer.isStarted())
        {
            mPrevFrame = mFrame;
            mFPSTimer.start();
        }
        
    }
    
    void GameManager::FPSStop()
    {
        if(mFPSTimer.isStarted())
        {
            mFPSTimer.stop();
        }
    }
    
    // ***********************************
    //           ゲームメイン処理
    // ***********************************
    void GameManager::changeState()
    {
        // プレイヤー状態
        mPlayer->changeState(mFrame);
        // FPS計測状態
        mFPSText.str( "" );
        mFPSText << std::fixed << std::setprecision(2) << mAvgFPS << "fps";
    }
    
    void GameManager::advanceFrame()
    {
        // フレームを進めて、平均FPSを記録
        mFrame++;
        mAvgFPS = (mFrame - mPrevFrame) / ( mFPSTimer.getTicks() / 1000.f );
    }
    
    void GameManager::saveMedia()
    {
        // セーブデータをゲームデータから一時保存領域へ
        mData[0] = mPlayer->getPosX();
        mData[1] = mPlayer->getPosY();
        mData[2] = mScene - 1;
        for(int i=3; i<mData.size(); ++i){
            mData[i] = 0;
        }
        
        // セーブデータを一時保存領域からファイルへ
        if(!saveData()){
            printf( "Error: Unable to dataSave! %s\n", SDL_GetError() );
        }
    }
    
    bool GameManager::saveData()
    {
        bool success = true;
        if( mScene != SCENE::INIT ){
            std::string path = mBasePath;// 実行ファイルディレクトリ取得
            path += "save/nums.dat";
            
            // バイナリファイルを書き込みモードで開く
            SDL_RWops* file = SDL_RWFromFile( path.c_str(), "w+b" );
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
        }
        return success;
    }
 
    void GameManager::cleanup()
    {
        // **** 開放処理 ****
        mData.clear();
        
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
    
    void GameManager::setSceneLoaded(bool loaded)
    {
        mSceneLoaded = loaded;
    }
    
    bool GameManager::getSceneLoaded()
    {
        return mSceneLoaded;
    }
}
