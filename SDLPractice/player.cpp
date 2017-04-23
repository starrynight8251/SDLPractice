//
//  player.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//
#include "player.h"
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "helper.h"
#include "gamemanager.h"
#include "graphicmanager.h"
#include "soundmanager.h"

namespace mygame {

Player::Player()
{
    mPosX = PLAYER_X;
    mPosY = PLAYER_Y;
    mDir  = DOWN;

    mVelX = mVelY = 0;
    mR = mG = mB = mA = 255;
    mDegrees = 0;
    mFlipType = SDL_FLIP_NONE;

    mColliders.resize( COLLISION_NUM );
    for(int i=0; i<COLLISION_NUM; i++){
        mColliders[i].w = collisions[i].w;
        mColliders[i].h = collisions[i].h;
    }
    shiftColliders();
    
    mPlayerTexture = new LTexture();
    mPlayerTexture->loadFromFile( "graphics/walk.png" );
    if( mPlayerTexture == NULL )
    {
        printf( "Failed to load walking animation texture!\n" );
    }
    else
    {
        mPlayerClips[ 0 ].x =   0;
        mPlayerClips[ 0 ].y =   0;
        mPlayerClips[ 0 ].w =  32;
        mPlayerClips[ 0 ].h =  32;
        
        mPlayerClips[ 1 ].x =  32;
        mPlayerClips[ 1 ].y =   0;
        mPlayerClips[ 1 ].w =  32;
        mPlayerClips[ 1 ].h =  32;
        
        mPlayerClips[ 2 ].x =  64;
        mPlayerClips[ 2 ].y =   0;
        mPlayerClips[ 2 ].w =  32;
        mPlayerClips[ 2 ].h =  32;
        
        mPlayerClips[ 3 ].x =  32;
        mPlayerClips[ 3 ].y =   0;
        mPlayerClips[ 3 ].w =  32;
        mPlayerClips[ 3 ].h =  32;
    }
    
    // パーティクル用領域確保
    particles.resize( GraphicManager::TOTAL_PARTICLES );
}

Player::~Player()
{
    mPlayerTexture->free();
    mPlayerTexture = NULL;
    // パーティクル解放
    for( int i = 0; i < GraphicManager::TOTAL_PARTICLES; ++i )
    {
        delete particles[ i ];
    }
    particles.clear();
}

void Player::handleEvent( SDL_Event& e )
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_LEFT:
                mDir = LEFT;
                mVelX -= PLAYER_VEL;
                break;
            case SDLK_RIGHT:
                mDir = RIGHT;
                mVelX += PLAYER_VEL;
                break;
            case SDLK_UP:
                mDir = UP;
                mVelY -= PLAYER_VEL;
                break;
            case SDLK_DOWN:
                mDir = DOWN;
                mVelY += PLAYER_VEL;
                break;

            // qwer,asdf キーでキャラクタの色を調整する
            case SDLK_q:
                // 色調整　赤　濃くする
                if( mR + 32 > 255 )
                {
                    mR = 255;
                }
                else
                {
                    mR += 32;
                }
                break;
                
                
            case SDLK_w:
                // 色調整　緑　濃くする
                if( mG + 32 > 255 )
                {
                    mG = 255;
                }
                else
                {
                    mG += 32;
                }
                break;
                
            case SDLK_e:
                // 色調整　青　濃くする
                if( mB + 32 > 255 )
                {
                    mB = 255;
                }
                else
                {
                    mB += 32;
                }
                break;
                
            case SDLK_r:
                // 不透明度　濃くする
                if( mA + 32 > 255 )
                {
                    mA = 255;
                }
                else
                {
                    mA += 32;
                }
                break;
                
            case SDLK_a:
                // 色調整　赤　薄くする
                if( mR - 32 < 0 )
                {
                    mR = 0;
                }
                else
                {
                    mR -= 32;
                }
                break;
                
            case SDLK_s:
                // 色調整　緑　薄くする
                if( mG - 32 < 0 )
                {
                    mG = 0;
                }
                else
                {
                    mG -= 32;
                }
                break;
                
            case SDLK_d:
                // 色調整　青　薄くする
                if( mB - 32 < 0 )
                {
                    mB = 0;
                }
                else
                {
                    mB -= 32;
                }
                break;
                
            case SDLK_f:
                // 不透明度　薄くする
                if( mA - 32 < 0 )
                {
                    mA = 0;
                }
                else
                {
                    mA -= 32;
                }
                break;
                
            case SDLK_t:
                // 反時計回り回転
                mDegrees -= 60;
                break;
                
            case SDLK_g:
                // 時計回り回転
                mDegrees += 60;
                break;
                
            case SDLK_y:
                // 左右反転
                mFlipType = SDL_FLIP_HORIZONTAL;
                break;
                
            case SDLK_h:
                // 反転なし
                mFlipType = SDL_FLIP_NONE;
                break;
                
            case SDLK_n:
                // 上下反転
                mFlipType = SDL_FLIP_VERTICAL;
                break;
        }
        mPlayerTexture->setColor(mR, mG, mB);
        mPlayerTexture->setAlpha(mA);
    }

    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
         switch( e.key.keysym.sym )
        {
                case SDLK_LEFT: mVelX += PLAYER_VEL; break;
                case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
                case SDLK_UP: mVelY += PLAYER_VEL; break;
                case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
        }
    }
    // コントローラ十字キー
    else if( e.type == SDL_JOYAXISMOTION )
    {
        //　コントローラ１
        if( e.jaxis.which == 0 )
        {
            // コントローラ左右キー
            if( e.jaxis.axis == 0 )
            {
                // 左
                if( e.jaxis.value < -GameManager::JOYSTICK_DEAD_ZONE )
                {
                    mDir = LEFT;
                    mVelX = -PLAYER_VEL;
                }
                // 右
                else if( e.jaxis.value > GameManager::JOYSTICK_DEAD_ZONE )
                {
                    mDir = RIGHT;
                    mVelX = PLAYER_VEL;
                }
                // 左右ニュートラル
                else
                {
                    mVelX = 0;
                }
            }
            // コントローラ上下キー
            else if( e.jaxis.axis == 1 )
            {
                // 上
                if( e.jaxis.value < -GameManager::JOYSTICK_DEAD_ZONE )
                {
                    mDir = UP;
                    mVelY = -PLAYER_VEL;
                }
                // 下
                else if( e.jaxis.value > GameManager::JOYSTICK_DEAD_ZONE )
                {
                    mDir = DOWN;
                    mVelY = PLAYER_VEL;
                }
                // 上下ニュートラル
                else
                {
                    mVelY = 0;
                }
            }
        }
    }
    // 向き補正
    if( mVelX < 0 && mVelY == 0 ) mDir = LEFT;
    if( mVelX > 0 && mVelY == 0 ) mDir = RIGHT;
    if( mVelX == 0 && mVelY < 0 ) mDir = UP;
    if( mVelX == 0 && mVelY > 0 ) mDir = DOWN;
}

void Player::move( std::vector<Tile*>& tiles )
{
    SoundManager* snd_manager = &SoundManager::getInstance();
    
    // 移動処理　X
    mPosX += mVelX;
    shiftColliders();

    // ウィンドウ境界と壁との当たり判定　X
    if( ( mPosX < 0 ) || ( mPosX + PLAYER_WIDTH > GraphicManager::LEVEL_WIDTH ) || touchesWall( mColliders, tiles ) )
    {
        // 壁に入ってしまうので戻す
        mPosX -= mVelX;
        shiftColliders();
        if(Mix_Playing(1) != 1){
            Mix_PlayChannel( 1, snd_manager->gLow, 0);
        }
    }

    // 移動処理 Y
    mPosY += mVelY;
    shiftColliders();
    
    // ウィンドウ境界と壁との当たり判定　Y
    if( ( mPosY < 0 ) || ( mPosY + PLAYER_HEIGHT > GraphicManager::LEVEL_HEIGHT ) || touchesWall( mColliders, tiles ) )
    {
        // 壁に入ってしまうので戻す
        mPosY -= mVelY;
        shiftColliders();
        if(Mix_Playing(1) != 1){
            Mix_PlayChannel( 1, snd_manager->gLow, 0);
        }
    }
}

void Player::renderParticles(SDL_Rect& camera){
    // パーティクル生成、再生成
    for( int i = 0; i < GraphicManager::TOTAL_PARTICLES; ++i )
    {
        //Delete and replace dead particles
        if( particles[ i ] == NULL || particles[ i ]->isDead() )
        {
            delete particles[ i ];
            particles[ i ] = new Particle( mPosX, mPosY );
        }
    }
    
    // パーティクル描画
    for( int i = 0; i < GraphicManager::TOTAL_PARTICLES; ++i )
    {
        particles[ i ]->render(camera);
    }

}

void Player::setCamera( SDL_Rect& camera )
{
    // カメラ位置設定
    camera.x = ( mPosX + PLAYER_WIDTH / 2 ) - GraphicManager::SCREEN_WIDTH / 2;
    camera.y = ( mPosY + PLAYER_HEIGHT / 2 ) - GraphicManager::SCREEN_HEIGHT / 2;
    
    // カメラを境界で止める
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > GraphicManager::LEVEL_WIDTH - camera.w )
    {
        camera.x = GraphicManager::LEVEL_WIDTH - camera.w;
    }
    if( camera.y > GraphicManager::LEVEL_HEIGHT - camera.h )
    {
        camera.y = GraphicManager::LEVEL_HEIGHT - camera.h;
    }
}

void Player::render( int frame, SDL_Rect& camera )
{
    // プレイヤーの表示
    int all_frames = WALKING_ANIM_DISPFRAME * WALKING_ANIM_CNT;
	SDL_Rect* currentClip = &mPlayerClips[ (frame % all_frames)/WALKING_ANIM_DISPFRAME ];
	currentClip->y = mDir*currentClip->w;
	mPlayerTexture->render( mPosX-camera.x, mPosY-camera.y, currentClip, mDegrees, NULL, mFlipType );
    renderParticles(camera);
}

int Player::getPosX(){
    return mPosX;
}

int Player::getPosY(){
    return mPosY;
}

void Player::setPosX(int posX){
    mPosX = posX;
}

void Player::setPosY(int posY){
    mPosY = posY;
}

void Player::shiftColliders(){
    for( int i = 0; i < mColliders.size(); i++ )
    {
        mColliders[i].x = mPosX + collisions[i].x;
        mColliders[i].y = mPosY + collisions[i].y;
   }
}

std::vector<SDL_Rect>& Player::getColliders()
{
    return mColliders;
}

}
