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
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// SDLの初期化、ウィンドウの作成
bool init();

// メディア読み込み
bool loadMedia();

//　メディア解放、ウィンドウクローズ
void close();

// bmp,jpg,png 画像のパスを指定する
// スクリーンフォーマットにコンバートしたサーフェースを返す
// 元画像からスクリーンフォーマットに毎回コンバートすると遅いので
SDL_Surface* loadSurface( std::string path );

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

// 描画用ウィンドウ
SDL_Window* gWindow = NULL;

// ウィンドウ内描画領域
SDL_Surface* gScreenSurface = NULL;

// 読込サーフェス
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

// ビットマップ用描画領域
SDL_Surface* gStretchedSurface = NULL;

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
            //Create renderer for window
            
            
            // JPG,PNG 読込のために SDL_image を初期化
            int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                success = false;
            }
            else
            {
                // ウィンドウ内描画領域取得
                gScreenSurface = SDL_GetWindowSurface( gWindow );
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
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "loaded.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }
    
    // 上画像
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "up.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }
    
    // 下画像
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "down.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }
    
    // 左画像
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "left.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }
    
    // 右画像
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "right.bmp" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }
    
    // ビットマップロード成功ならtrue
    return success;
}

void close()
{
    // サーフェス解放
    for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
    {
        SDL_FreeSurface( gKeyPressSurfaces[ i ] );
        gKeyPressSurfaces[ i ] = NULL;
    }
    
    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    
    // SDL終了
    IMG_Quit();
    SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
    // スクリーンフォーマットのサーフェス
    SDL_Surface* optimizedSurface = NULL;
    
    // 画像読み込みし一時的にサーフェス確保
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        // サーフェスをスクリーンフォーマットにコンバートする
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        // 一時的に確保したサーフェスを解放
        SDL_FreeSurface( loadedSurface );
    }
    
    // スクリーンフォーマットのサーフェスを戻す
    return optimizedSurface;
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
            
            //Set default current surface
            gStretchedSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
            
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
                                gStretchedSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                                break;
                                
                            case SDLK_DOWN:
                                gStretchedSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                                break;
                                
                            case SDLK_LEFT:
                                gStretchedSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                                break;
                                
                            case SDLK_RIGHT:
                                gStretchedSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                                break;
                                
                            default:
                                gStretchedSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                                break;
                        }
                    }

                }
                
                //　画像をスクリーンに書き込み
                SDL_BlitSurface( gStretchedSurface, NULL, gScreenSurface, NULL );
                
                // 元画像を引き延ばしてスクリーンに描画
                SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = SCREEN_WIDTH;
                stretchRect.h = SCREEN_HEIGHT;
                SDL_BlitScaled( gStretchedSurface, NULL, gScreenSurface, &stretchRect );
                
                //　スクリーン書き換え
                SDL_UpdateWindowSurface( gWindow );
            }
        }
    }
    
    // 終了処理
    close();
    
    return 0;
}
