#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <string>
#include "ltexture.h"

// スクリーンの幅と高さを定義
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

// キー操作表示用テクスチャID
enum KeyPressTextures
{
    KEY_PRESS_TEXTURE_DEFAULT,
    KEY_PRESS_TEXTURE_UP,
    KEY_PRESS_TEXTURE_DOWN,
    KEY_PRESS_TEXTURE_LEFT,
    KEY_PRESS_TEXTURE_RIGHT,
    KEY_PRESS_TEXTURE_TOTAL
};

// ウィンドウ, レンダラ, SDL, SDL_image の初期化
bool init();
// データ読み込み
bool loadMedia();
//　メディア解放、ウィンドウクローズ
void close();

// 描画用ウィンドウ
SDL_Window* gWindow = NULL;
// 描画用レンダラ
SDL_Renderer* gRenderer = NULL;

// キー操作表示用テクスチャ
LTexture* gKeyPressTextures[ KEY_PRESS_TEXTURE_TOTAL ];
LTexture* gPresentTexture = NULL;
// スプライト表示用テクスチャ
LTexture* gSpriteSheetTexture = NULL;
SDL_Rect gSpriteClips[ 4 ];
// 色抜表示用テクスチャ
LTexture* gColorKeyTexture = NULL;

bool init()
{
    // 成功フラグ
    bool success = true;
    
    // SDL初期化
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        // 初期化失敗
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // テクスチャフィルタリングを線形にする
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        // SDL初期化成功、ウィンドウ作成
        gWindow = SDL_CreateWindow( "starrynight8251",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            // ウィンドウ作成失敗
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // ウィンドウに対してレンダラを作成する
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                // JPG,PNG 読込のために SDL_image を初期化
                int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                else
                {
                    // レンダラのクリア色を設定する
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    
                    // SDL_image で jpg, png を読込可能にする
                    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
                    if( !( IMG_Init( imgFlags ) & imgFlags ) )
                    {
                        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                        success = false;
                    }
                    
                    // **** メモリ割当 ****
                    for (int i=0; i<KEY_PRESS_TEXTURE_TOTAL; i++) {
                        gKeyPressTextures[i] = new LTexture();
                    }
                    gSpriteSheetTexture = new LTexture();
                    gColorKeyTexture = new LTexture();
                }
            }
        }
    }
    
    // SDL初期化,ウィンドウ作成ともに成功ならtrue
    return success;
}

bool loadMedia()
{
    // 成功フラグ
    bool success = true;
    
    // キー操作表示用テクスチャ読込
    // 初期テクスチャ
    gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ]->loadFromFile( "loaded.png" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }
    
    // 上テクスチャ
    gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ]->loadFromFile( "up.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }
    
    // 下テクスチャ
    gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ]->loadFromFile( "down.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }
    
    // 左テクスチャ
    gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ]->loadFromFile( "left.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }
    
    // 右テクスチャ
    gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ]->loadFromFile( "right.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }

    // スプライト表示用テクスチャ読込
    gSpriteSheetTexture->loadFromFile( "dots.png" );
    if( gSpriteSheetTexture == NULL )
    {
        printf( "Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        //**** アルファブレンディング設定 ****
        gSpriteSheetTexture->setBlendMode( SDL_BLENDMODE_BLEND );

        // 左上スプライト位置
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w = 100;
        gSpriteClips[ 0 ].h = 100;
        
        // 右上スプライト位置
        gSpriteClips[ 1 ].x = 100;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w = 100;
        gSpriteClips[ 1 ].h = 100;
        
        // 左下スプライト位置
        gSpriteClips[ 2 ].x =   0;
        gSpriteClips[ 2 ].y = 100;
        gSpriteClips[ 2 ].w = 100;
        gSpriteClips[ 2 ].h = 100;
        
        // 右下スプライト位置
        gSpriteClips[ 3 ].x = 100;
        gSpriteClips[ 3 ].y = 100;
        gSpriteClips[ 3 ].w = 100;
        gSpriteClips[ 3 ].h = 100;
    }
    
    // 色抜表示用テクスチャ読込
    gColorKeyTexture->loadFromFile( "foo.png" );
    if( gColorKeyTexture == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }

    // ビットマップロード成功ならtrue
    return success;
}

void close()
{
    // **** メモリ解放 ****
    for (int i=0; i<KEY_PRESS_TEXTURE_TOTAL; i++) {
        gKeyPressTextures[i]->free();
        gKeyPressTextures[i] = NULL;
    }
    gSpriteSheetTexture->free();
    gSpriteSheetTexture = NULL;
    gColorKeyTexture->free();
    gColorKeyTexture = NULL;

    // レンダラ,ウィンドウを破棄する
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;
    
    // SDL, SDL_image の解放
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    // SDL初期化、ウィンドウ作成
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        // 画像読み込み
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            // 周期イベントフラグ
            bool quit = false;
            
            // イベントハンドラ
            SDL_Event ev;
            
            // キー操作表示用テクスチャをデフォルトのテクスチャに設定
            gPresentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ];
            
            // スプライト表示用テクスチャの色調整
            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;
            Uint8 a = 255;
            
            // 色抜テクスチャ表示位置
            int gX = 0;
            int gY = 0;
            
            // 周期イベント
            while( !quit )
            {
                //　イベントを読み取る
                while( SDL_PollEvent( &ev ) != 0 )
                {
                    //　ウィンドウを閉じる
                    if( ev.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    
                    // キーダウンイベント処理
                    else if( ev.type == SDL_KEYDOWN )
                    {
                        gPresentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ];
                        
                        switch( ev.key.keysym.sym )
                        {
                            // キー操作で表示テクスチャを切り替える
                            // 色抜テクスチャの表示位置を移動する
                            case SDLK_UP:
                                gPresentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ];
                                gY -= 100;
                                break;
                                
                            case SDLK_DOWN:
                                gPresentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ];
                                gY += 100;
                                break;
                                
                            case SDLK_LEFT:
                                gPresentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ];
                                gX -= 100;
                                break;
                                
                            case SDLK_RIGHT:
                                gPresentTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ];
                                gX += 100;
                                break;
                            
                            // qwer,asdf キーでスプライト表示用テクスチャの色を調整する
                            case SDLK_q:
                                // 色調整　赤　濃くする
                                if( r + 32 > 255 )
                                {
                                    r = 255;
                                }
                                else
                                {
                                    r += 32;
                                }
                                break;

                                
                            case SDLK_w:
                                // 色調整　緑　濃くする
                                if( g + 32 > 255 )
                                {
                                    g = 255;
                                }
                                else
                                {
                                    g += 32;
                                }
                                break;
                                
                            case SDLK_e:
                                // 色調整　青　濃くする
                                if( b + 32 > 255 )
                                {
                                    b = 255;
                                }
                                else
                                {
                                    b += 32;
                                }
                                break;
                                
                            case SDLK_r:
                                // 不透明度　濃くする
                                if( a + 32 > 255 )
                                {
                                    a = 255;
                                }
                                else
                                {
                                    a += 32;
                                }
                                break;
                                
                            case SDLK_a:
                                // 色調整　赤　薄くする
                                if( r - 32 < 0 )
                                {
                                    r = 0;
                                }
                                else
                                {
                                    r -= 32;
                                }
                                break;
                                
                            case SDLK_s:
                                // 色調整　緑　薄くする
                                if( g - 32 < 0 )
                                {
                                    g = 0;
                                }
                                else
                                {
                                    g -= 32;
                                }
                                break;
                                
                            case SDLK_d:
                                // 色調整　青　薄くする
                                if( b - 32 < 0 )
                                {
                                    b = 0;
                                }
                                else
                                {
                                    b -= 32;
                                }
                                break;
                                
                            case SDLK_f:
                                // 不透明度　薄くする
                                if( a - 32 < 0 )
                                {
                                    a = 0;
                                }
                                else
                                {
                                    a -= 32;
                                }
                                break;
                        }
                    }
                }
                
                // **** 前処理 ****
                //　設定したクリア色でクリアする
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                
                // ビューポートをスクリーン全体に設定
                SDL_Rect normalViewport;
                normalViewport.x = 0;
                normalViewport.y = 0;
                normalViewport.w = SCREEN_WIDTH;
                normalViewport.h = SCREEN_HEIGHT;
                SDL_RenderSetViewport( gRenderer, &normalViewport );
                
                
                // **** キー操作表示用テクスチャ描画 ****
                gPresentTexture->render(0,0);
                
                // **** スプライト表示用テクスチャ描画 ****
                // 色調整設定
                gSpriteSheetTexture->setColor( r, g, b );
                gSpriteSheetTexture->setAlpha( a );
                
                // 左上スプライト描画
                gSpriteSheetTexture->render( 0, 0, &gSpriteClips[ 0 ] );
                // 右上スプライト描画
                gSpriteSheetTexture->render( SCREEN_WIDTH - gSpriteClips[ 1 ].w, 0, &gSpriteClips[ 1 ] );
                // 左下スプライト描画
                gSpriteSheetTexture->render( 0, SCREEN_HEIGHT - gSpriteClips[ 2 ].h, &gSpriteClips[ 2 ] );
                // 右下スプライト描画
                gSpriteSheetTexture->render( SCREEN_WIDTH - gSpriteClips[ 3 ].w, SCREEN_HEIGHT - gSpriteClips[ 3 ].h, &gSpriteClips[ 3 ] );
                
                // **** プリミティブ描画 ****
                // 赤の塗りつぶし四角を描画
                SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                SDL_RenderFillRect( gRenderer, &fillRect );
                
                //　緑の四角を描画
                SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
                SDL_RenderDrawRect( gRenderer, &outlineRect );
                
                // 青の水平線を描画
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
                SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );
                
                // 黄色の点を垂直に描画
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
                for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
                {
                    SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
                }
                
                // **** 色抜表示用テクスチャ描画 ****
                gColorKeyTexture->render(gX, gY);
                
                // **** 後処理 ****
                // オフスクリーンバッファからスクリーンに転送
                SDL_RenderPresent( gRenderer );
                
            }
        }
    }
    
    // 終了処理
    close();
    
    return 0;
}
