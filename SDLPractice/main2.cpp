#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <sstream>

#include "ltexture.h"
#include "lbutton.h"
#include "ltimer.h"
#include "dot.h"
#include "player.h"

extern const int SCREEN_WIDTH = 640;
extern const int SCREEN_HEIGHT = 480;
// コントローラアナログスティックの無反応範囲
extern const int JOYSTICK_DEAD_ZONE = 8000;

bool init();
bool loadMedia();
void close();
extern bool cneckCollision( SDL_Rect a, SDL_Rect b );


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// **** グラフィック ****
// キャラクタ
LTexture* gPlayerTexture = NULL;
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gPlayerClips[ WALKING_ANIMATION_FRAMES ];

// ボタン
//Button constants
extern const int BUTTON_WIDTH = 60;
extern const int BUTTON_HEIGHT = 40;
extern const int TOTAL_BUTTONS = 4;

LTexture* gButtonTexture = NULL;
LButton gButtons[ TOTAL_BUTTONS ];
SDL_Rect gButtonClips[ BUTTON_SPRITE_TOTAL ];

// テキスト
LTexture* gDotTexture = NULL;

// テキスト
LTexture* gTextTexture = NULL;
TTF_Font* gFont = NULL;

// **** サウンド　****
// サウンド
Mix_Music *gMusic = NULL;
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

// **** 操作 ****
// コントローラ１
SDL_Joystick* gGameController = NULL;
SDL_Haptic* gControllerHaptic = NULL;

bool init()
{
    bool success = true;
	
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
        // ウィンドウ作成
        gWindow = SDL_CreateWindow( "starrynight8251",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // レンダラ作成
            // SDL_RENDERER_ACCELERATED ハードウェア支援機能有り
            // SDL_RENDERER_PRESENTVSYNC 画面の更新タイミングを待つ(垂直同期)
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED |SDL_RENDERER_PRESENTVSYNC );
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

    // **** メモリ割当 ****
    gPlayerTexture = new LTexture();
    gButtonTexture = new LTexture();
    gTextTexture = new LTexture();
    gDotTexture = new LTexture();
    return success;
}

bool loadMedia()
{
    bool success = true;
 
    // キャラクタ
    gPlayerTexture->loadFromFile( "graphics/walk.png" );
    if( gPlayerTexture == NULL )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        gPlayerClips[ 0 ].x =   0;
        gPlayerClips[ 0 ].y =   0;
        gPlayerClips[ 0 ].w =  32;
        gPlayerClips[ 0 ].h =  32;
        
        gPlayerClips[ 1 ].x =  32;
        gPlayerClips[ 1 ].y =   0;
        gPlayerClips[ 1 ].w =  32;
        gPlayerClips[ 1 ].h =  32;
        
        gPlayerClips[ 2 ].x =  64;
        gPlayerClips[ 2 ].y =   0;
        gPlayerClips[ 2 ].w =  32;
        gPlayerClips[ 2 ].h =  32;
        
        gPlayerClips[ 3 ].x =  32;
        gPlayerClips[ 3 ].y =   0;
        gPlayerClips[ 3 ].w =  32;
        gPlayerClips[ 3 ].h =  32;
    }

    // ボタン
    if( !gButtonTexture->loadFromFile( "graphics/button.png" ) )
    {
        printf( "Failed to load button sprite texture!\n" );
        success = false;
    }
    else
    {
        //Set sprites
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
        {
            gButtonClips[ i ].x = 0;
            gButtonClips[ i ].y = i * 200;
            gButtonClips[ i ].w = BUTTON_WIDTH;
            gButtonClips[ i ].h = BUTTON_HEIGHT;
        }
        
        //Set buttons in corners
        gButtons[ 0 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH*4,
                                  SCREEN_HEIGHT - BUTTON_HEIGHT*2 );
        gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH*3,
                                  SCREEN_HEIGHT - BUTTON_HEIGHT*2 );
        gButtons[ 2 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH*2,
                                  SCREEN_HEIGHT - BUTTON_HEIGHT*2 );
        gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH,
                                  SCREEN_HEIGHT - BUTTON_HEIGHT*2 );
    }
    
    //Load dot texture
    if( !gDotTexture->loadFromFile( "graphics/dot.bmp" ) )
    {
        printf( "Failed to load dot texture!\n" );
        success = false;
    }
    
    // テキスト
    gFont = TTF_OpenFont( "fonts/lazy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    
    // サウンド
    gMusic = Mix_LoadMUS( "sounds/mario.mp3" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gScratch = Mix_LoadWAV( "sounds/scratch.wav" );
    if( gScratch == NULL )
    {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gHigh = Mix_LoadWAV( "sounds/high.wav" );
    if( gHigh == NULL )
    {
        printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gMedium = Mix_LoadWAV( "sounds/medium.wav" );
    if( gMedium == NULL )
    {
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    gLow = Mix_LoadWAV( "sounds/low.wav" );
    if( gLow == NULL )
    {
        printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    
    // ビットマップロード成功ならtrue
    return success;
}

void close()
{
    // 再生中かチェック
    if( Mix_PlayingMusic() != 0 )
    {
        // 一時停止していなければ一時停止する
        if( Mix_PausedMusic() != 1 )
        {
            Mix_PauseMusic();
        }
    }
    // 停止しておく
    Mix_HaltMusic();

    // **** メモリ解放 ****
    // グラフィック
    gPlayerTexture->free();
    gPlayerTexture = NULL;
    gButtonTexture->free();
    gButtonTexture = NULL;
    gTextTexture->free();
    gTextTexture = NULL;
    gDotTexture->free();
    gDotTexture = NULL;
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    // サウンド
    Mix_FreeChunk( gScratch );
    Mix_FreeChunk( gHigh );
    Mix_FreeChunk( gMedium );
    Mix_FreeChunk( gLow );
    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    
    // 操作
    SDL_HapticClose( gControllerHaptic );
    SDL_JoystickClose( gGameController );
    gGameController = NULL;
    gControllerHaptic = NULL;

    // レンダラ,ウィンドウ
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gRenderer = NULL;
    gWindow = NULL;
    
    // SDL_ttf, SDL_mixer, SDL_image, SDL の解放
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}


int main( int argc, char* args[] )
{
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            bool quit = false;
            
            SDL_Event ev;

            // キャラクタ位置
            int px = 0;
            int py = 0;
            // キャラクタ位置差分
            int dx = 0;
            int dy = 0;
            // キャラクタ向き
            int dir = 3;
            
            // キャラクタ回転角
            double degrees = 0;
            // キャラクタ反転
            SDL_RendererFlip flipType = SDL_FLIP_NONE;
            
            // キャラクタの色調整
            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;
            Uint8 a = 255;
            
            //The dot that will be moving around on the screen
            Dot dot;
            Player player;
            
            //Set the wall
            SDL_Rect wall;
            wall.x = 320;
            wall.y = 64;
            wall.w = 128;
            wall.h = 320;
            
            // テキストカラー
            SDL_Color textColor = { 255, 0, 255 };
            
            //The frames per second timer
            LTimer fpsTimer;
            
            //In memory text stream
            std::stringstream timeText;
            
            //Start counting frames per second
            int frame = 0;
            fpsTimer.start();

            // メインイベントループ
            while( !quit )
            {
                // イベント判別処理
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
                        switch( ev.key.keysym.sym )
                        {
                            // 矢印 キーでキャラクタの位置、向きを変更する
                            case SDLK_LEFT:
                                px -= 100;
                                dir = 0;
                                break;
                                
                            case SDLK_RIGHT:
                                px += 100;
                                dir = 1;
                                break;
                                
                            case SDLK_UP:
                                py -= 100;
                                dir = 2;
                                break;
                                
                            case SDLK_DOWN:
                                py += 100;
                                dir = 3;
                                break;
                            
                            // qwer,asdf キーでキャラクタの色を調整する
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
                                
                            case SDLK_t:
                                // 反時計回り回転
                                degrees -= 60;
                                break;
                                
                            case SDLK_g:
                                // 時計回り回転
                                degrees += 60;
                                break;
                                
                            case SDLK_y:
                                // 左右反転
                                flipType = SDL_FLIP_HORIZONTAL;
                                break;
                                
                            case SDLK_h:
                                // 上下反転
                                flipType = SDL_FLIP_NONE;
                                break;
                                
                            case SDLK_n:
                                // 反転なし
                                flipType = SDL_FLIP_VERTICAL;
                                break;
                                
                            case SDLK_u:
                                if( fpsTimer.isStarted() )
                                {
                                    fpsTimer.stop();
                                }
                                else
                                {
                                    fpsTimer.start();
                                }
                                break;
                                
                            case SDLK_j:
                                if( fpsTimer.isPaused() )
                                {
                                    fpsTimer.unpause();
                                }
                                else
                                {
                                    fpsTimer.pause();
                                }
                                break;
                                
                        }
                    }
                    // コントローラ十字キー
                    else if( ev.type == SDL_JOYAXISMOTION )
                    {
                        //　コントローラ１
                        if( ev.jaxis.which == 0 )
                        {
                            // コントローラ左右キー
                            if( ev.jaxis.axis == 0 )
                            {
                                // 左
                                if( ev.jaxis.value < -JOYSTICK_DEAD_ZONE )
                                {
                                    dir = 0;
                                    dx = -5;
                                }
                                // 右
                                else if( ev.jaxis.value > JOYSTICK_DEAD_ZONE )
                                {
                                    dir = 1;
                                    dx = 5;
                                }
                                // 左右ニュートラル
                                else
                                {
                                    dx = 0;
                                }
                            }
                            // コントローラ上下キー
                            else if( ev.jaxis.axis == 1 )
                            {
                                // 上
                                if( ev.jaxis.value < -JOYSTICK_DEAD_ZONE )
                                {
                                    dir = 2;
                                    dy = -5;
                                }
                                // 下
                                else if( ev.jaxis.value > JOYSTICK_DEAD_ZONE )
                                {
                                    dir = 3;
                                    dy =  5;
                                }
                                // 上下ニュートラル
                                else
                                {
                                    dy = 0;
                                }
                            }
                        }
                    }
                    
                    // コントローラボタン
                    else if( ev.type == SDL_JOYBUTTONDOWN)
                    {
                        // コントローラ１
                        if( ev.jbutton.which == 0 )
                        {
                            // 各ボタン番号で分岐
                            switch ( ev.jbutton.button+1 ) {
                                case 1:
                                    Mix_PlayChannel( -1, gHigh, 0 );
                                    break;
                                    
                                case 2:
                                    Mix_PlayChannel( -1, gMedium, 0 );
                                    break;
                                    
                                case 3:
                                    Mix_PlayChannel( -1, gLow, 0 );
                                    break;
                                    
                                case 4:
                                    Mix_PlayChannel( -1, gScratch, 0 );
                                    break;
                                    
                                case 9:
                                    // 音楽が停止中なら
                                    if( Mix_PlayingMusic() == 0 )
                                    {
                                        // 音楽を再生
                                        Mix_PlayMusic( gMusic, -1 );
                                    }
                                    // 音楽が再生中なら
                                    else
                                    {
                                        // 一時停止中なら
                                        if( Mix_PausedMusic() == 1 )
                                        {
                                            // 一時停止解除
                                            Mix_ResumeMusic();
                                        }
                                        // 一時停止していなければ
                                        else
                                        {
                                            // 一時停止
                                            Mix_PauseMusic();
                                        }
                                    }
                                    break;
                                    
                                case 10:
                                    // 音楽を停止
                                    Mix_HaltMusic();
                                    break;
                            }
                        }
                    }

                    // ボタン判定処理（マウスクリック、マウスオーバー）
                    for( int i = 0; i < TOTAL_BUTTONS; ++i )
                    {
                        gButtons[ i ].handleEvent( &ev );
                    }
                    
                    //Handle input for the dot
                    dot.handleEvent( ev );
                    player.handleEvent( ev );
                }
                
                // **** 更新処理　****
                //Calculate and correct fps
                float avgFPS = frame / ( fpsTimer.getTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                    avgFPS = 0;
                }
                //Set text to be rendered
                timeText.str( "" );
                timeText << "Average Frames Per Second " << avgFPS;
                
                //Render text
                if( !gTextTexture->loadFromRenderedText( timeText.str().c_str(), textColor ) )
                {
                    printf( "Unable to render FPS texture!\n" );
                }
                
                // キャラクタ表示位置更新
                px += dx;
                py += dy;
                
                //Move the dot
                dot.move( wall );
                player.move( wall );
                
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
                
                // **** グラフィック描画 ****
//                // 赤の塗りつぶし四角を描画
//                SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
//                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
//                SDL_RenderFillRect( gRenderer, &fillRect );
//                
//                //　緑の四角を描画
//                SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
//                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
//                SDL_RenderDrawRect( gRenderer, &outlineRect );
//                
//                // 青の水平線を描画
//                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
//                SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2 + 20, SCREEN_WIDTH, SCREEN_HEIGHT / 2 + 20);
//                
//                // 黄色の点を垂直に描画
//                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
//                for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
//                {
//                    SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
//                }

                //Render wall
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderDrawRect( gRenderer, &wall );
                
                // キャラクタ描画
                gPlayerTexture->setColor( r, g, b );
                gPlayerTexture->setAlpha( a );
                
                int dir_off[] = { 3, 1, 0, 2 };
                SDL_Rect* currentClip = &gPlayerClips[ (frame % 16)/4 ];
                currentClip->y = dir_off[dir]*32;
                gPlayerTexture->render( px, py, 4, currentClip, degrees, NULL, flipType );
                
                //Render objects
                player.render( frame );
                dot.render();
                
                // テキスト描画
                gTextTexture->render( ( SCREEN_WIDTH - gTextTexture->getWidth() ) / 2, SCREEN_HEIGHT - gTextTexture->getHeight() );
                
                // ボタン描画
                for( int i = 0; i < TOTAL_BUTTONS; ++i )
                {
                    gButtons[ i ].render();
                }
 
                // 描きこまれた裏側スクリーンを表側スクリーンに転送
                SDL_RenderPresent( gRenderer );
 
                // 次フレーム
                ++frame;
            }
        }
    }
    
    // 終了処理
    close();
    
    return 0;
}
