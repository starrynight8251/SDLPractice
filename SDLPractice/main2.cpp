#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_net/SDL_net.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <stdio.h>
#include <string>

// スクリーンの幅と高さを定義
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// 読込サーフェスID
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

// テクスチャをロードする
// bmp, jpg, png を読込可能
SDL_Texture* loadTexture( std::string path );

// 描画用ウィンドウ
SDL_Window* gWindow = NULL;

// 読込テクスチャ
SDL_Texture* gKeyPressTextures[ KEY_PRESS_TEXTURE_TOTAL ];

// 描画用レンダラ
SDL_Renderer* gRenderer = NULL;

//　表示用テクスチャ
SDL_Texture* gTexture = NULL;


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
    
    // 初期画像
    gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ] = loadTexture( "loaded.png" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }
    
    // 上画像
    gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ] = loadTexture( "up.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }
    
    // 下画像
    gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ] = loadTexture( "down.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }
    
    // 左画像
    gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ] = loadTexture( "left.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }
    
    // 右画像
    gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ] = loadTexture( "right.bmp" );
    if( gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }
    
    // ビットマップロード成功ならtrue
    return success;
}

void close()
{
    // 読込テクスチャを解放する
    for (int i=0; i<KEY_PRESS_TEXTURE_TOTAL; i++) {
        SDL_DestroyTexture(gKeyPressTextures[i]) ;
        gKeyPressTextures[i] = NULL;
    }

    // レンダラ,ウィンドウを破棄する
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;
    
    // SDL, SDL_image の解放
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
    // 返却用のテクスチャ
    SDL_Texture* newTexture = NULL;
    
    // テクスチャをロードする
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        // サーフェス上のピクセルからテクスチャを作成する
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        // テクスチャに読み込んだのでサーフェスは解放する
        SDL_FreeSurface( loadedSurface );
    }
    // テクスチャ返却
    return newTexture;
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
            
            // 表示用テクスチャをデフォルトのテクスチャに設定
            gTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ];
            
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
                    // キーを押した時、画像を変更する
                    else if( ev.type == SDL_KEYDOWN )
                    {
                        switch( ev.key.keysym.sym )
                        {
                            case SDLK_UP:
                                gTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_UP ];
                                break;
                                
                            case SDLK_DOWN:
                                gTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DOWN ];
                                break;
                                
                            case SDLK_LEFT:
                                gTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_LEFT ];
                                break;
                                
                            case SDLK_RIGHT:
                                gTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_RIGHT ];
                                break;
                                
                            default:
                                gTexture = gKeyPressTextures[ KEY_PRESS_TEXTURE_DEFAULT ];
                                break;
                        }
                    }
                    
                }
                
                //　設定したクリア色でクリアする
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                
                // ビューポートを左上四角に設定
                SDL_Rect topLeftViewport;
                topLeftViewport.x = 0;
                topLeftViewport.y = 0;
                topLeftViewport.w = SCREEN_WIDTH / 2;
                topLeftViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport( gRenderer, &topLeftViewport );
                
                // テクスチャを描画
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

                // ビューポートを右上四角に設定
                SDL_Rect topRightViewport;
                topRightViewport.x = SCREEN_WIDTH / 2;
                topRightViewport.y = 0;
                topRightViewport.w = SCREEN_WIDTH / 2;
                topRightViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport( gRenderer, &topRightViewport );
                
                //Render texture to screen
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                
                // ビューポートを下四角に設定
                SDL_Rect bottomViewport;
                bottomViewport.x = 0;
                bottomViewport.y = SCREEN_HEIGHT / 2;
                bottomViewport.w = SCREEN_WIDTH;
                bottomViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport( gRenderer, &bottomViewport );
                
                //Render texture to screen
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                
                // ビューポートをスクリーン全体に設定
                SDL_Rect normalViewport;
                normalViewport.x = 0;
                normalViewport.y = 0;
                normalViewport.w = SCREEN_WIDTH;
                normalViewport.h = SCREEN_HEIGHT;
                SDL_RenderSetViewport( gRenderer, &normalViewport );
                
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
                
                // レンダラからスクリーンに転送
                SDL_RenderPresent( gRenderer );

            }
        }
    }
    
    // 終了処理
    close();
    
    return 0;
}
