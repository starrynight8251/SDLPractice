//
//  graphicmanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#include "graphicmanager.h"
#include <fstream>
#include <sstream>
#include "gamemanager.h"

namespace mygame{// start of namespace
bool GraphicManager::init()
{
    gPlayer = NULL;
    gPersonTexture = NULL;
    gTileTexture = NULL;
    gTextTexture = NULL;
    gBGTexture = NULL;
    gRedTexture = NULL;
    gGreenTexture = NULL;
    gBlueTexture = NULL;
    gShimmerTexture = NULL;
    
    gFont = NULL;

    gPersonTexture = new LTexture();
    gTileTexture = new LTexture();
    gTextTexture = new LTexture();
    gBGTexture = new LTexture();
    gRedTexture = new LTexture();
    gGreenTexture = new LTexture();
    gBlueTexture = new LTexture();
    gShimmerTexture = new LTexture();
    
    gTiles.resize( TOTAL_TILES );
    gTileClips.resize( TOTAL_TILE_SPRITES );
    
    return true;
}

bool GraphicManager::loadMedia()
{
    bool success = true;
    
    if( !gPersonTexture->loadFromFile( "graphics/walk.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
    }
    
    // タイルマップ用テクスチャ読込
    if( !gTileTexture->loadFromFile( "graphics/tiles2.png" ) )
    {
        printf( "Failed to load tile set texture!\n" );
        success = false;
    }
    
    // タイルマップ読込
    if( !setTiles( gTiles ) )
    {
        printf( "Failed to load tile set!\n" );
        success = false;
    }
    
    // パーティクル赤
    if( !gRedTexture->loadFromFile( "graphics/red.bmp" ) )
    {
        printf( "Failed to load red texture!\n" );
        success = false;
    }
    
    // パーティクル緑
    if( !gGreenTexture->loadFromFile( "graphics/green.bmp" ) )
    {
        printf( "Failed to load green texture!\n" );
        success = false;
    }
    
    // パーティクル青
    if( !gBlueTexture->loadFromFile( "graphics/blue.bmp" ) )
    {
        printf( "Failed to load blue texture!\n" );
        success = false;
    }
    
    // パーティクル輝
    if( !gShimmerTexture->loadFromFile( "graphics/shimmer.bmp" ) )
    {
        printf( "Failed to load shimmer texture!\n" );
        success = false;
    }
    
    // アルファ値設定
    gRedTexture->setAlpha( 192 );
    gGreenTexture->setAlpha( 192 );
    gBlueTexture->setAlpha( 192 );
    gShimmerTexture->setAlpha( 192 );
    
    
    // 背景
    //if( !gBGTexture->loadFromFile( "graphics/BG.png" ) )
    if( !gBGTexture->loadFromFile( "graphics/sweetbear_1280_960_f.jpg" ) )
    {
        printf( "Failed to load background texture!\n" );
        success = false;
    }
    
    // テキスト
    gFont = TTF_OpenFont( "fonts/ipagp-mona.ttf", 18 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    gPlayer = new Player();

    return success;
}

void GraphicManager::setPlayerPos(int x, int y){
    gPlayer->setPosX(x);
    gPlayer->setPosY(y);
}
    
bool GraphicManager::setTiles(std::vector<Tile*>& tiles)
{
    bool tilesLoaded = true;
    
    // タイルのオフセット
    int x = 0, y = 0;
    
    // マップ用ファイルを開く
    std::ifstream map( "graphics/lazy.map" );
    if( !map )
    {
        printf( "Unable to load map file!\n" );
        tilesLoaded = false;
    }
    else
    {
        // タイルを初期化
        for( int i = 0; i < TOTAL_TILES; ++i )
        {
            int tileType = -1;
            
            // マップ用ファイルからタイル情報を読込
            map >> tileType;
            
            // 読込中に問題発生
            if( map.fail() )
            {
                // 読込を停止する
                printf( "Error loading map: Unexpected end of file!\n" );
                tilesLoaded = false;
                break;
            }
            
            // タイル番号が適正なら
            if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
            {
                tiles[ i ] = new Tile( x, y, tileType );
            }
            // タイル番号が不適正なら
            else
            {
                // 読込を停止する
                printf( "Error loading map: Invalid tile type at %d!\n", i );
                tilesLoaded = false;
                break;
            }
            
            // 次のタイルへ
            x += TILE_WIDTH;
            
            // 右端まで行ったら
            if( x >= LEVEL_WIDTH )
            {
                // 左端に戻る
                x = 0;
                
                // 次の列へ
                y += TILE_HEIGHT;
            }
        }
        
        //　クリップ用四角形を定義
        if( tilesLoaded )
        {
            gTileClips[ TILE_RED ].x = 0;
            gTileClips[ TILE_RED ].y = 0;
            gTileClips[ TILE_RED ].w = TILE_WIDTH;
            gTileClips[ TILE_RED ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_GREEN ].x = 0;
            gTileClips[ TILE_GREEN ].y = 80;
            gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
            gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_BLUE ].x = 0;
            gTileClips[ TILE_BLUE ].y = 160;
            gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
            gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_TOPLEFT ].x = 80;
            gTileClips[ TILE_TOPLEFT ].y = 0;
            gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
            gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_LEFT ].x = 80;
            gTileClips[ TILE_LEFT ].y = 80;
            gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
            gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_BOTTOMLEFT ].x = 80;
            gTileClips[ TILE_BOTTOMLEFT ].y = 160;
            gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
            gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_TOP ].x = 160;
            gTileClips[ TILE_TOP ].y = 0;
            gTileClips[ TILE_TOP ].w = TILE_WIDTH;
            gTileClips[ TILE_TOP ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_CENTER ].x = 160;
            gTileClips[ TILE_CENTER ].y = 80;
            gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
            gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_BOTTOM ].x = 160;
            gTileClips[ TILE_BOTTOM ].y = 160;
            gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
            gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_TOPRIGHT ].x = 240;
            gTileClips[ TILE_TOPRIGHT ].y = 0;
            gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
            gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_RIGHT ].x = 240;
            gTileClips[ TILE_RIGHT ].y = 80;
            gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
            gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;
            
            gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
            gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
            gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
            gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
        }
    }
    
    // ファイルを閉じる
    map.close();
    
    return tilesLoaded;
}

void GraphicManager::render( int frame )
{
    GameManager* gm_manager = &GameManager::getInstance();
    
    // **** 描画前処理 ****
    //　設定したクリア色でクリアする
    SDL_SetRenderDrawColor( gm_manager->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gm_manager->gRenderer );

    // 背景
    gBGTexture->render( 0, 0, &camera);
    
    //Render level
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        gTiles[ i ]->render( camera );
    }
    
    // キャラクタ
    gPlayer->render( frame, camera );
    
    // FPS
    gTextTexture->render( SCREEN_WIDTH - gTextTexture->getWidth(), 0 );
    
    // **** 描画後処理 ****
    // 描きこまれた裏側スクリーンを表側スクリーンに転送
    SDL_RenderPresent( gm_manager->gRenderer );
}

void GraphicManager::handleEvent(SDL_Event e)
{
    gPlayer->handleEvent(e);
}
    
void GraphicManager::move(int frame)
{
    gPlayer->move(gTiles);
}
    
void GraphicManager::setCamera(){
    gPlayer->setCamera( camera );
}

void GraphicManager::setFPS(std::stringstream& timeText, SDL_Color& textColor)
{
    if( !gTextTexture->loadFromRenderedText( timeText.str(), textColor ) )
    {
        printf( "Unable to render FPS texture!\n" );
    }
}

void GraphicManager::setData(std::vector<int>& data)
{
    data[0] = gPlayer->getPosX();
    data[1] = gPlayer->getPosY();
    for(int i=2; i<data.size(); ++i){
        data[i] = 0;
    }
}

void GraphicManager::cleanup(){
    delete gPlayer;
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        if( gTiles[ i ] != NULL )
        {
            delete gTiles[ i ];
            gTiles[ i ] = NULL;
        }
    }

    delete gPersonTexture;
    delete gTileTexture;
    delete gTextTexture;
    delete gBGTexture;
    delete gRedTexture;
    delete gGreenTexture;
    delete gBlueTexture;
    delete gShimmerTexture;
    TTF_CloseFont( gFont );
    gFont = NULL;
}
}// end of namespace
