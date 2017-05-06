//
//  graphicmanager.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "graphicmanager.h"
#include <fstream>
#include <sstream>
#include "sdlmanager.h"
#include "gamemanager.h"
#include "helper.h"

namespace mygame{
    
    bool GraphicManager::init()
    {
        mCamera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        mFont = NULL;
        return true;
    }
    
    bool GraphicManager::loadMedia( GameManager::SCENE scene )
    {
        bool success = true;
        
        GameManager* gm_manager = &GameManager::getInstance();
        std::string base_path = gm_manager->BASE_PATH;
        std::string grp_path  = gm_manager->GRP_PATH;
        std::string fnt_path  = gm_manager->FNT_PATH;
        std::string path;
        
        if( scene == GameManager::SCENE::INIT ){
            // 背景
            // if( !mBGTextures.back()->loadFromFile( "graphics/BG.png" ) )
            path = base_path + grp_path + "sweetbear_1280_960_f.jpg";
            mBGTextures.push_back(new LTexture());
            if( !mBGTextures.back()->loadFromFile( path.c_str() ) )
            {
                printf( "Failed to load background texture!\n" );
                success = false;
            }
            
            // マップ用テクスチャ読込
            path = base_path + grp_path + "tiles2.png";
            mMapSheetTextures.push_back(new LTexture());
            if( !mMapSheetTextures.back()->loadFromFile( path.c_str() ) )
            {
                printf( "Failed to load tile set texture!\n" );
                success = false;
            }
            
            // スプライト用テクスチャ読込
            path = base_path + grp_path + "walk.png";
            mSpriteSheetTextures.push_back(new LTexture());
            if( !mSpriteSheetTextures.back()->loadFromFile( path.c_str() ) )
            {
                printf( "Failed to load walking animation texture!\n" );
            }
            
            // パーティクル用テクスチャ読込
            // パーティクル赤
            path = base_path + grp_path + "red.bmp";
            mParticleTextures.push_back(new LTexture());
            if( !mParticleTextures.back()->loadFromFile( path.c_str() ) )
            {
                printf( "Failed to load red texture!\n" );
                success = false;
            }
            // パーティクル緑
            path = base_path + grp_path + "green.bmp";
            mParticleTextures.push_back(new LTexture());
            if( !mParticleTextures.back()->loadFromFile( path.c_str() ) )
            {
                printf( "Failed to load green texture!\n" );
                success = false;
            }
            // パーティクル青
            path = base_path + grp_path + "blue.bmp";
            mParticleTextures.push_back(new LTexture());
            if( !mParticleTextures.back()->loadFromFile( path.c_str() ) )
            {
                printf( "Failed to load blue texture!\n" );
                success = false;
            }
            // パーティクル輝
            path = base_path + grp_path + "shimmer.bmp";
            mParticleTextures.push_back(new LTexture());
            if( !mParticleTextures.back()->loadFromFile( path.c_str() ) )
            {
                printf( "Failed to load shimmer texture!\n" );
                success = false;
            }
            // アルファ値設定
            for (int i=0; i < (int)mParticleTextures.size(); ++i) {
                mParticleTextures[i]->setAlpha( 192 );
            }
            
            // テキスト用テクスチャ
            path = base_path + fnt_path + "ipagp-mona.ttf";
            mTextTextures.push_back(new LTexture());
            mFont = TTF_OpenFont( path.c_str(), 18 );
            if( mFont == NULL )
            {
                printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
                success = false;
            }
        }
        
        return success;
    }
    
    // ***********************************
    //         グラフィックメイン処理
    // ***********************************
    void GraphicManager::update()
    {
        SDLManager* sdl_manager = &SDLManager::getInstance();
        GameManager* gm_manager = &GameManager::getInstance();
        
        SDL_Renderer* gm_renderer = sdl_manager->getRenderer();
        std::vector<Tile*> gm_map = gm_manager->getMap();
        Player* plyer = gm_manager->getPlayer();
        
        // カメラ位置設定
        setCamera(plyer->getPosX(),plyer->getPosY(),Player::PLAYER_WIDTH,Player::PLAYER_HEIGHT);
        
        // **** 描画前処理 ****
        //　設定したクリア色でクリアする
        SDL_SetRenderDrawColor( gm_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gm_renderer );
        
        // **** 背景 ****
        mBGTextures[0]->render( 0, 0, &mCamera);
        
        // **** マップ描画 ****
        int chips[] = {0,1,2,7,6,9,10,11,8,5,4,3};
        for( int i = 0; i < (int)gm_map.size(); ++i )
        {
            Tile* pTile = gm_map[i];
            
            // タイルがスクリーン上にあるなら（クリッピング）
            if( checkCollision( mCamera, pTile->getBox() ) )
            {
                int w = Tile::TILE_WIDTH;
                int h = Tile::TILE_HEIGHT;
                // テクスチャクリップ位置指定
                SDL_Rect currentClip = {w*(chips[pTile->getType()]/3),h*(chips[pTile->getType()]%3),w,h};
                // タイル描画
                mMapSheetTextures[0]->render( pTile->getBox().x - mCamera.x, pTile->getBox().y - mCamera.y, &currentClip );
            }
        }
        
        // **** スプライト描画 ****
        // テクスチャ色設定
        int sheetIndex = plyer->getSpriteSheetIndex();
        int animIndex  = plyer->getSpriteAnimIndex();
        int dirIndex   = plyer->getDir();
        mSpriteSheetTextures[sheetIndex]->setColor(plyer->getRed(), plyer->getGreen(), plyer->getBlue());
        mSpriteSheetTextures[sheetIndex]->setAlpha(plyer->getAlpha());
        
        // テクスチャクリップ位置設定
        int dir[] = { 3, 1, 0, 2 };
        int anim[] = { 0, 1, 2, 1};
        int w = Player::PLAYER_WIDTH;
        int h = Player::PLAYER_HEIGHT;
        SDL_Rect currentClip = {w*anim[animIndex],h*dir[dirIndex],w,h};
        
        // スプライト描画
        mSpriteSheetTextures[sheetIndex]->render(plyer->getPosX()-mCamera.x, plyer->getPosY()-mCamera.y, &currentClip);
        
        // パーティクル描画
        for( int i = 0; i < Player::TOTAL_PARTICLES; ++i )
        {
            std::vector<Particle*> part = plyer->getParticles();
            // パーティクル描画
            if( part[i]->getLife() % 2 == 0 )
            {
                // パーティクル輝　描画
                mParticleTextures[3]->render( part[i]->getPosX()-mCamera.x, part[i]->getPosY()-mCamera.y );
            }else{
                // パーティクル赤、緑、青　描画
                mParticleTextures[part[i]->getTexIndex()]->render( part[i]->getPosX() - mCamera.x, part[i]->getPosY() - mCamera.y);
            }
            part[i]->decLife();
        }
        
        // **** FPSテキスト描画 ****
        SDL_Color FPSTextColor = { 255, 0, 255 };
        setText(gm_manager->getFPSText(), FPSTextColor);
        mTextTextures[0]->render( SCREEN_WIDTH - mTextTextures[0]->getWidth(), 0 );
        
        // **** 描画後処理 ****
        // 描きこまれた裏側スクリーンを表側スクリーンに転送
        SDL_RenderPresent( gm_renderer );
    }
    
    void GraphicManager::setCamera( int px, int py, int pw, int ph )
    {
        // カメラ位置設定
        mCamera.x = ( px + pw / 2 ) - SCREEN_WIDTH / 2;
        mCamera.y = ( py + ph / 2 ) - SCREEN_HEIGHT / 2;
        
        // カメラを境界で止める
        if( mCamera.x < 0 )
        {
            mCamera.x = 0;
        }
        if( mCamera.y < 0 )
        {
            mCamera.y = 0;
        }
        if( mCamera.x > GameManager::MAP_WIDTH - mCamera.w )
        {
            mCamera.x = GameManager::MAP_WIDTH - mCamera.w;
        }
        if( mCamera.y > GameManager::MAP_HEIGHT - mCamera.h )
        {
            mCamera.y = GameManager::MAP_HEIGHT - mCamera.h;
        }
        
    }
    
    void GraphicManager::setText(std::stringstream& text, SDL_Color& textColor)
    {
        if( !mTextTextures[0]->loadFromRenderedText( text.str(), textColor ) )
        {
            printf( "Unable to render FPS texture!\n" );
        }
    }
    
    void GraphicManager::cleanup()
    {
        for (int i = 0; i < (int)mBGTextures.size(); ++i) {
            if( mBGTextures[i] != NULL )
            {
                delete mBGTextures[ i ];
                mBGTextures[ i ] = NULL;
            }
        }
        mBGTextures.clear();
        
        for (int i = 0; i < (int)mMapSheetTextures.size(); ++i) {
            if( mMapSheetTextures[i] != NULL )
            {
                delete mMapSheetTextures[ i ];
                mMapSheetTextures[ i ] = NULL;
            }
        }
        mMapSheetTextures.clear();
        
        for (int i = 0; i < (int)mSpriteSheetTextures.size(); ++i) {
            if( mSpriteSheetTextures[i] != NULL )
            {
                delete mSpriteSheetTextures[ i ];
                mSpriteSheetTextures[ i ] = NULL;
            }
        }
        mSpriteSheetTextures.clear();
        
        for (int i = 0; i < (int)mParticleTextures.size(); ++i) {
            if( mParticleTextures[i] != NULL )
            {
                delete mParticleTextures[ i ];
                mParticleTextures[ i ] = NULL;
            }
        }
        mParticleTextures.clear();
        
        for (int i = 0; i < (int)mTextTextures.size(); ++i) {
            if( mTextTextures[i] != NULL )
            {
                delete mTextTextures[ i ];
                mTextTextures[ i ] = NULL;
            }
        }
        mTextTextures.clear();
        
        TTF_CloseFont( mFont );
        mFont = NULL;
    }
    
}
