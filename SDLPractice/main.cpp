#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <sstream>
#include <iomanip>
#include <vector>

#include "lwindow.h"
#include "ltexture.h"
#include "lbutton.h"
#include "ltimer.h"
#include "player.h"

// 背景画像サイズ
extern const int LEVEL_WIDTH = 1280;
extern const int LEVEL_HEIGHT = 960;

// スクリーンサイズ
extern const int SCREEN_WIDTH = 640;
extern const int SCREEN_HEIGHT = 480;

// コントローラアナログスティックの無反応範囲
extern const int JOYSTICK_DEAD_ZONE = 8000;

bool init();
bool loadMedia();
void close();

LWindow* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// **** グラフィック ****
LTexture* gBGTexture;

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
    gBGTexture = new LTexture();
    gTextTexture = new LTexture();
    return success;
}

bool loadMedia()
{
    bool success = true;
    
    // 背景
    if( !gBGTexture->loadFromFile( "graphics/BG.png" ) )
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
    
    if( Mix_PlayingMusic() == 0 )
    {
        // 音楽を再生
        Mix_PlayMusic( gMusic, -1 );
    }

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
    gBGTexture->free();
    gBGTexture = NULL;
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
            
            // 現在のフレーム
            int frame = 0;
            
            // FPS計測用
            LTimer fpsTimer;// タイマー
            int prev_frame = 0;// 最小化したときのフレーム
            float avgFPS = 0.0f;// FPS計算結果
            SDL_Color textColor = { 255, 0, 255 };// テキストカラー
            std::stringstream timeText;// 表示用テキスト
            
            // カメラ位置
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            
            // プレイヤー
            Player player;
            
            // 当たり判定用の壁
            std::vector<SDL_Rect> walls;
            walls.resize( 3 );
            walls[0].x = 320;
            walls[0].y = 64;
            walls[0].w = 128;
            walls[0].h = 120;
            
            walls[1].x = 0;
            walls[1].y = 0;
            walls[1].w = 128;
            walls[1].h = 120;
            
            walls[2].x = 64;
            walls[2].y = 240;
            walls[2].w = 128;
            walls[2].h = 120;
            
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
                    // プレイヤー用イベント判別処理
                    if( !gWindow->isMinimized() )
                    {
                        player.handleEvent( ev );
                    }
                    // ウィンドウ用イベント判別処理
                    gWindow->handleEvent( ev );
                }
                
                // ウィンドウを最小化した時
                if( gWindow->isMinimized() )
                {
                    // FPS計測を再開する
                    if(fpsTimer.isStarted())
                    {
                        fpsTimer.stop();
                    }
                    // 音楽を再開する
                    // 再生中かチェック
                    if( Mix_PlayingMusic() != 0 )
                    {
                        // 一時停止していなければ一時停止する
                        if( Mix_PausedMusic() != 1 )
                        {
                            Mix_PauseMusic();
                        }
                    }
                }
                // 通常ウィンドウの時
                else
                {
                    // FPS計測中かチェック
                    if(!fpsTimer.isStarted())
                    {
                        // FPS計測が止まっていれば開始する
                        prev_frame = frame;
                        fpsTimer.start();
                    }

                    // 再生中かチェック
                    if( Mix_PlayingMusic() != 0 )
                    {
                        // 一時停止していれば再開する
                        if( Mix_PausedMusic() == 1 )
                        {
                            Mix_ResumeMusic();
                        }
                    }
                    
                    // **** 更新処理　****
                    // プレイヤー・カメラ位置更新
                    player.move( walls );
                    
                    camera.x = ( player.getPosX() + Player::PLAYER_WIDTH / 2 ) - SCREEN_WIDTH / 2;
                    camera.y = ( player.getPosY() + Player::PLAYER_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;
                    
                    if( camera.x < 0 )
                    {
                        camera.x = 0;
                    }
                    if( camera.y < 0 )
                    {
                        camera.y = 0;
                    }
                    if( camera.x > LEVEL_WIDTH - camera.w )
                    {
                        camera.x = LEVEL_WIDTH - camera.w;
                    }
                    if( camera.y > LEVEL_HEIGHT - camera.h )
                    {
                        camera.y = LEVEL_HEIGHT - camera.h;
                    }
                    
                    // FPS情報更新
                    timeText.str( "" );
                    timeText << std::fixed << std::setprecision(2) << avgFPS << "fps";
                    if( !gTextTexture->loadFromRenderedText( timeText.str(), textColor ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                    
                    // **** 描画前処理 ****
                    //　設定したクリア色でクリアする
                    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                    SDL_RenderClear( gRenderer );
                    
                    // **** 描画処理 ****
                    // 背景
                    gBGTexture->render( 0, 0, &camera);
                    
                    // キャラクタ
                    player.render( frame, camera.x, camera.y );
                    
                    // 壁
                    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                    for(int i=0; i<walls.size(); i++){
                        SDL_Rect renderRect = walls[i];
                        renderRect.x -= camera.x;
                        renderRect.y -= camera.y;
                        SDL_RenderDrawRect(gRenderer, &renderRect);
                    }
                    
                    // FPS
                    gTextTexture->render( SCREEN_WIDTH - gTextTexture->getWidth(), 0 );
               
                    // **** 描画後処理 ****
                    // 描きこまれた裏側スクリーンを表側スクリーンに転送
                    SDL_RenderPresent( gRenderer );
                    // 次フレーム
                    ++frame;
                    // 秒間フレーム数計算
                    avgFPS = (frame - prev_frame) / ( fpsTimer.getTicks() / 1000.f );
                    
                }
            }
        }
    }
    
    // 終了処理
    close();
    return 0;
}
