//
//  ltexture.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "ltexture.h"
#include "sdlmanager.h"
#include "graphicmanager.h"

namespace mygame{
    
    LTexture::LTexture()
    {
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
    
    LTexture::~LTexture()
    {
        free();
    }
    
    bool LTexture::loadFromFile( std::string path )
    {
        SDLManager* sdl_manager = &SDLManager::getInstance();
        
        // すでに確保されていたら解放する
        free();
        
        // パスにある画像ファイルから一時サーフェス作成
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == NULL )
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        }
        else
        {
            // 色抜き設定
            SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
            
            // サーフェスからテクスチャを作成する
            mTexture = SDL_CreateTextureFromSurface( sdl_manager->getRenderer(), loadedSurface );
            if( mTexture == NULL )
            {
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
            else
            {
                // 画像のサイズを取得
                mWidth  = loadedSurface->w;
                mHeight = loadedSurface->h;
            }
            
            // テクスチャを作成したら必要ないので一時サーフェスを解放する
            SDL_FreeSurface( loadedSurface );
        }
        
        // テクスチャを設定
        return mTexture != NULL;
    }
    
#ifdef _SDL_TTF_H
    bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
    {
        SDLManager* sdl_manager = &SDLManager::getInstance();
        GraphicManager* grp_manager = &GraphicManager::getInstance();
        
        // すでに確保されていたら解放する
        free();
        
        // テキストから一時サーフェス作成
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(grp_manager->mFont, textureText.c_str(), textColor);
        if( textSurface == NULL )
        {
            printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        }
        else
        {
            // サーフェスからテクスチャを作成する
            mTexture = SDL_CreateTextureFromSurface( sdl_manager->getRenderer(), textSurface );
            if( mTexture == NULL )
            {
                printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                // 画像のサイズを取得
                mWidth = textSurface->w;
                mHeight = textSurface->h;
            }
            
            // テクスチャを作成したら必要ないので一時サーフェスを解放する
            SDL_FreeSurface( textSurface );
        }
        
        return mTexture != NULL;
    }
#endif
    
    void LTexture::free()
    {
        // テクスチャが存在していたら解放する
        if( mTexture != NULL )
        {
            SDL_DestroyTexture( mTexture );
            mTexture = NULL;
            mWidth = 0;
            mHeight = 0;
        }
    }
    
    // 色調整設定
    void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
    {
        // 色調整
        SDL_SetTextureColorMod( mTexture, red, green, blue );
    }
    
    // 色混合モード設定
    void LTexture::setBlendMode( SDL_BlendMode blending )
    {
        // 色混合モード
        SDL_SetTextureBlendMode( mTexture, blending );
    }
    
    // アルファ値設定
    void LTexture::setAlpha( Uint8 alpha )
    {
        // アルファ値
        SDL_SetTextureAlphaMod( mTexture, alpha );
    }
    
    
    void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
    {
        SDLManager* sdl_manager = &SDLManager::getInstance();
        // スクリーン表示位置
        SDL_Rect renderQuad = { x, y, mWidth, mHeight };
        
        // クリップ画像サイズ
        if( clip != NULL )
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }
        
        // スクリーンに描画する
        SDL_RenderCopyEx( sdl_manager->getRenderer(), mTexture, clip, &renderQuad, angle, center, flip );
    }
    
    int LTexture::getWidth()
    {
        return mWidth;
    }
    
    int LTexture::getHeight()
    {
        return mHeight;
    }
    
}
