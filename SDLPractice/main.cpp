#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include "lwindow.h"
#include "ltexture.h"
#include "lbutton.h"
#include "ltimer.h"
#include "player.h"
#include "tile.h"

// 背景画像サイズ
extern const int LEVEL_WIDTH = 1280;
extern const int LEVEL_HEIGHT = 960;

// スクリーンサイズ
extern const int SCREEN_WIDTH = 640;
extern const int SCREEN_HEIGHT = 480;

// コントローラアナログスティックの無反応範囲
extern const int JOYSTICK_DEAD_ZONE = 8000;


// タイルマップ用定数
extern const int TILE_WIDTH = 80;
extern const int TILE_HEIGHT = 80;
extern const int TOTAL_TILES = 192;
extern const int TOTAL_TILE_SPRITES = 12;

// タイルの種別
extern const int TILE_RED = 0;
extern const int TILE_GREEN = 1;
extern const int TILE_BLUE = 2;
extern const int TILE_CENTER = 3;
extern const int TILE_TOP = 4;
extern const int TILE_TOPRIGHT = 5;
extern const int TILE_RIGHT = 6;
extern const int TILE_BOTTOMRIGHT = 7;
extern const int TILE_BOTTOM = 8;
extern const int TILE_BOTTOMLEFT = 9;
extern const int TILE_LEFT = 10;
extern const int TILE_TOPLEFT = 11;

// パーティクル（粒子）の数
extern const int TOTAL_PARTICLES = 20;

// セーブ用データ数
const int TOTAL_DATA = 10;
// セーブ用データ
Sint32 gData[ TOTAL_DATA ];

bool init();
bool loadMedia();
void close();
bool setTiles(Tile *tiles[]);

LWindow* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

// **** グラフィック ****
Player* gPlayer = NULL;
LTexture* gTileTexture;
LTexture* gTextTexture = NULL;
LTexture* gBGTexture;
LTexture* gRedTexture;
LTexture* gGreenTexture;
LTexture* gBlueTexture;
LTexture* gShimmerTexture;
std::vector<SDL_Rect> gTileClips;
TTF_Font* gFont = NULL;

// **** サウンド　****
Mix_Music *gMusic = NULL;
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

// **** コントローラー ****
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
    // グローバル用、後で整理する!!
    gPlayer = new Player();
    gTileTexture = new LTexture();
    gTextTexture = new LTexture();
    gBGTexture = new LTexture();
    gRedTexture = new LTexture();
    gGreenTexture = new LTexture();
    gBlueTexture = new LTexture();
    gShimmerTexture = new LTexture();

    gTileClips.resize( TOTAL_TILE_SPRITES );
    return success;
}

bool loadMedia( Tile* tiles[] )
{
    bool success = true;
    
    // **** データロード ****
    // バイナリファイルを読み込みモードで開く
    SDL_RWops* file = SDL_RWFromFile( "save/nums.bin", "r+b" );
    
    // ファイルが存在しなかった
    if( file == NULL )
    {
        printf( "Warning: Unable to open file! SDL Error: %s\n", SDL_GetError() );
        
        // バイナリファイルを書き込みモードで開く（新規作成）
        file = SDL_RWFromFile( "save/nums.bin", "w+b" );
        if( file != NULL )
        {
            printf( "New file created!\n" );
            
            // データ初期化
            for( int i = 0; i < TOTAL_DATA; ++i )
            {
                gData[ i ] = 0;
                SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
            }
            
            // ファイルを閉じる
            SDL_RWclose( file );
        }
        else
        {
            printf( "Error: Unable to create file! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
    }
    // ファイルが存在する
    else
    {
        // データをロードする
        printf( "Reading file...!\n" );
        for( int i = 0; i < TOTAL_DATA; ++i )
        {
            SDL_RWread( file, &gData[ i ], sizeof(Sint32), 1 );
        }
        
        // ファイルを閉じる
        SDL_RWclose( file );
        
        // 読み込んだデータを設定する
        // 今のところはキャラクタの位置のみ
        gPlayer->setPosX(gData[0]);
        gPlayer->setPosY(gData[1]);
    }

    // タイルマップ用テクスチャ読込
    if( !gTileTexture->loadFromFile( "graphics/tiles2.png" ) )
    {
        printf( "Failed to load tile set texture!\n" );
        success = false;
    }
    
    // タイルマップ読込
    if( !setTiles( tiles ) )
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

void close( Tile* tiles[] )
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
    
    // **** データセーブ ****
    // データを書込用に設定する
    gData[0] = gPlayer->getPosX();
    gData[1] = gPlayer->getPosY();
    for(int i=2; i<TOTAL_DATA; ++i){
        gData[i] = 0;
    }
    
    // バイナリファイルを書き込みモードで開く
    SDL_RWops* file = SDL_RWFromFile( "save/nums.bin", "w+b" );
    if( file != NULL )
    {
        // データをセーブする
        for( int i = 0; i < TOTAL_DATA; ++i )
        {
            SDL_RWwrite( file, &gData[ i ], sizeof(Sint32), 1 );
        }
        
        // ファイルを閉じる
        SDL_RWclose( file );
    }
    else
    {
        printf( "Error: Unable to save file! %s\n", SDL_GetError() );
    }
    

    // **** メモリ解放 ****
    delete gPlayer;
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        if( tiles[ i ] != NULL )
        {
            delete tiles[ i ];
            tiles[ i ] = NULL;
        }
    }
    // グラフィック
    delete gTileTexture;
    delete gTextTexture;
    delete gBGTexture;
    delete gRedTexture;
    delete gGreenTexture;
    delete gBlueTexture;
    delete gShimmerTexture;
    TTF_CloseFont( gFont );
    gFont = NULL;
    
    // サウンド
    Mix_FreeChunk( gScratch );
    Mix_FreeChunk( gHigh );
    Mix_FreeChunk( gMedium );
    Mix_FreeChunk( gLow );
    Mix_FreeMusic( gMusic );
    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;
    gMusic = NULL;
    
    // コントローラ
    SDL_HapticClose( gControllerHaptic );
    SDL_JoystickClose( gGameController );
    gGameController = NULL;
    gControllerHaptic = NULL;

    // レンダラ,ウィンドウ
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;
    delete gWindow;
    
    // SDL_ttf, SDL_mixer, SDL_image, SDL の解放
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}


bool setTiles( Tile* tiles[] )
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


int main( int argc, char* args[] )
{
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        // タイルマップ
        Tile* tileSet[ TOTAL_TILES ];
        
        if( !loadMedia( tileSet ) )
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
                        gPlayer->handleEvent( ev );
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
                    gPlayer->move( tileSet );
                    gPlayer->setCamera( camera );
                    
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
                    
                    //Render level
                    for( int i = 0; i < TOTAL_TILES; ++i )
                    {
                        tileSet[ i ]->render( camera );
                    }
                    
                    // キャラクタ
                    gPlayer->render( frame, camera );
             
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
        // 終了処理
        close( tileSet );
    }
    
    return 0;
}
