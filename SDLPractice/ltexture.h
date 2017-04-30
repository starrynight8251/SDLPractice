//
//  ltexture.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef ltexture_h
#define ltexture_h

#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <string>

namespace mygame{
    
    class LTexture
    {
    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
    
    public:
        LTexture();
        ~LTexture();
    
        bool loadFromFile( std::string path );
    
#ifdef _SDL_TTF_H
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
#endif
    
        void free();
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        
        // アクセス関数
        void setColor( Uint8 red, Uint8 green, Uint8 blue );
        void setBlendMode( SDL_BlendMode blending );
        void setAlpha( Uint8 alpha );
        int getWidth();
        int getHeight();
    };
    
}

#endif /* ltexture_h */
