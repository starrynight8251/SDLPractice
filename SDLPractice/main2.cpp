#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <sstream>
#include <iomanip>

#include "lwindow.h"
#include "ltexture.h"
#include "lbutton.h"
#include "ltimer.h"
#include "player.h"

using namespace std;

extern const int SCREEN_WIDTH = 640;
extern const int SCREEN_HEIGHT = 480;
// コントローラアナログスティックの無反応範囲
extern const int JOYSTICK_DEAD_ZONE = 8000;

bool init();
bool loadMedia();
void close();
extern bool cneckCollision( SDL_Rect a, SDL_Rect b );

//SDL_Window* gWindow = NULL;
//Our custom window
LWindow* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// **** グラフィック ****
// ボタン
extern const int BUTTON_WIDTH = 60;
extern const int BUTTON_HEIGHT = 40;
extern const int TOTAL_BUTTONS = 4;

LTexture* gButtonTexture = NULL;
LButton gButtons[ TOTAL_BUTTONS ];
SDL_Rect gButtonClips[ BUTTON_SPRITE_TOTAL ];

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
        gWindow = new LWindow();
        
        // ウィンドウ作成
        if( !gWindow->init() )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // レンダラ作成
            // SDL_RENDERER_ACCELERATED ハードウェア支援機能有り
            // SDL_RENDERER_PRESENTVSYNC 画面の更新タイミングを待つ(垂直同期)
            gRenderer = gWindow->createRenderer();
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
    gButtonTexture = new LTexture();
    gTextTexture = new LTexture();
    return success;
}

bool loadMedia()
{
    bool success = true;
 
    // ボタン
    if( !gButtonTexture->loadFromFile( "graphics/button.png" ) )
    {
        printf( "Failed to load button sprite texture!\n" );
        success = false;
    }
    else
    {
        // マウス用ボタンのソース画像クリップ位置指定
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
        {
            gButtonClips[ i ].x = 0;
            gButtonClips[ i ].y = i * 200;
            gButtonClips[ i ].w = BUTTON_WIDTH;
            gButtonClips[ i ].h = BUTTON_HEIGHT;
        }
        
        // マウス用ボタンの位置指定
        gButtons[ 0 ].setPosition( gWindow->getWidth() - BUTTON_WIDTH*4,
                                  gWindow->getHeight() - BUTTON_HEIGHT*2 );
        gButtons[ 1 ].setPosition( gWindow->getWidth() - BUTTON_WIDTH*3,
                                  gWindow->getHeight() - BUTTON_HEIGHT*2 );
        gButtons[ 2 ].setPosition( gWindow->getWidth() - BUTTON_WIDTH*2,
                                  gWindow->getHeight() - BUTTON_HEIGHT*2 );
        gButtons[ 3 ].setPosition( gWindow->getWidth() - BUTTON_WIDTH,
                                  gWindow->getHeight() - BUTTON_HEIGHT*2 );
    }
    
    // テキスト
    gFont = TTF_OpenFont( "fonts/ipagp-mona.ttf", 18 );
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
    gButtonTexture->free();
    gButtonTexture = NULL;
    gTextTexture->free();
    gTextTexture = NULL;
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
    gWindow->free();
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
    // 四角形の各辺
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // 四角形aの各辺の座標を計算する
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    // 四角形bの各辺の座標を計算する
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    // Aの辺のいずれかがBの外側なら重なっていない
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

    // Aのどの辺もBの外側でない
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
            
            // プレイヤー
            Player player;
            
            // 当たり判定用の壁
            SDL_Rect wall;
            wall.x = 320;
            wall.y = 64;
            wall.w = 128;
            wall.h = 320;
            
            // テキストカラー
            SDL_Color textColor = { 255, 0, 255 };
            
            // FPS計測用
            std::stringstream timeText;
            int frame = 0;
            int prev_frame = 0;
            float avgFPS = 0.0f;
            LTimer fpsTimer;
            
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
                            }
                            // コントローラ上下キー
                            else if( ev.jaxis.axis == 1 )
                            {
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

                    // ボタン用イベント判別（マウスクリック、マウスオーバー）
                    for( int i = 0; i < TOTAL_BUTTONS; ++i )
                    {
                        gButtons[ i ].handleEvent( &ev );
                    }
                    
                    // プレイヤー用イベント判別
                    player.handleEvent( ev );
                    // ウィンドウ用イベント判別
                    gWindow->handleEvent( ev );
                }
                
                // 最小化されていればタイマーを止めて
                // 最小化されていなければタイマを再開
                if( gWindow->isMinimized() )
                {
                    if(fpsTimer.isStarted())
                    {
                        fpsTimer.stop();
                        prev_frame = frame;
                    }
                }else
                {
                    if( !fpsTimer.isStarted() )
                    {
                        fpsTimer.start();
                    }
                }
                
                // 最小化されていないときのみ描画
                if( !gWindow->isMinimized() )
                {
                    // **** 更新処理　****
                    timeText.str( "" );
                    timeText << fixed << setprecision(2) << avgFPS << "fps";
                    if( !gTextTexture->loadFromRenderedText( timeText.str(), textColor ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                    
                    // キャラクタ表示位置更新
                    player.move( wall );
                    
                    // **** 前処理 ****
                    //　設定したクリア色でクリアする
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderClear( gRenderer );
                    
                    // ビューポートをスクリーン全体に設定
                    SDL_Rect normalViewport;
                    normalViewport.x = 0;
                    normalViewport.y = 0;
                    normalViewport.w = gWindow->getWidth();
                    normalViewport.h = gWindow->getHeight();
                    SDL_RenderSetViewport( gRenderer, &normalViewport );
                    
                    // **** グラフィック描画 ****
                    // 当たり判定用の壁
                    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                    SDL_RenderDrawRect( gRenderer, &wall );
                    
                    // キャラクタ描画
                    player.render( frame );
                    
                    // FPS描画
                    gTextTexture->render( SCREEN_WIDTH - gTextTexture->getWidth(), 0 );
                    
                    // ボタン描画
                    for( int i = 0; i < TOTAL_BUTTONS; ++i )
                    {
                        gButtons[ i ].render();
                    }
                    
                    // 描きこまれた裏側スクリーンを表側スクリーンに転送
                    SDL_RenderPresent( gRenderer );
                    
                    // 次フレーム
                    ++frame;

                    // 秒間フレーム数計算
                    avgFPS = (frame-prev_frame) / ( fpsTimer.getTicks() / 1000.f );
                    
                }
            }
        }
    }
    
    // 終了処理
    close();
    
    return 0;
}
