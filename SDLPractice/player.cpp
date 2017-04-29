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
        // ゲーム内状態
        mPosX = PLAYER_X; mPosY = PLAYER_Y;
        mVelX = mVelY = 0;
        mDir  = DOWN;
        
        mColliders.resize( COLLISION_NUM );
        for(int i=0; i<COLLISION_NUM; i++){
            mColliders[i].w = collisions[i].w;
            mColliders[i].h = collisions[i].h;
        }
        shiftColliders();
        
        // 表示用状態
        mSpriteSheetIndex = 0;
        mSheetCharaIndex = 0;
        mSheetAnimIndex = 0;
        mR = mG = mB = mA = 255;
        mDegrees = 0;
        mFlipType = SDL_FLIP_NONE;
        
        // パーティクル用領域確保
        mParticles.resize( TOTAL_PARTICLES );
    }
    
    Player::~Player()
    {
        // パーティクル解放
        for( int i = 0; i < TOTAL_PARTICLES; ++i )
        {
            delete mParticles[ i ];
        }
        mParticles.clear();
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
        if( mVelX < 0 && mVelY == 0 ){
            mDir = LEFT;
        }
        if( mVelX > 0 && mVelY == 0 ){
            mDir = RIGHT;
        }
        if( mVelX == 0 && mVelY < 0 ){
            mDir = UP;
        }
        if( mVelX == 0 && mVelY > 0 ){
            mDir = DOWN;
        }
    }
    
    void Player::move( int frame, std::vector<Tile*>& tiles )
    {
        SoundManager* snd_manager = &SoundManager::getInstance();
        
        // 移動処理　X
        mPosX += mVelX;
        shiftColliders();
        
        // ウィンドウ境界と壁との当たり判定　X
        if( ( mPosX < 0 ) || ( mPosX + PLAYER_WIDTH > GameManager::MAP_WIDTH ) || touchesWall( mColliders, tiles ) )
        {
            // 壁に入ってしまうので戻す
            mPosX -= mVelX;
            shiftColliders();
            if(Mix_Playing(1) != 1){
                Mix_PlayChannel( 1, snd_manager->mLow, 0);
            }
        }
        
        // 移動処理 Y
        mPosY += mVelY;
        shiftColliders();
        
        // ウィンドウ境界と壁との当たり判定　Y
        if( ( mPosY < 0 ) || ( mPosY + PLAYER_HEIGHT > GameManager::MAP_HEIGHT ) || touchesWall( mColliders, tiles ) )
        {
            // 壁に入ってしまうので戻す
            mPosY -= mVelY;
            shiftColliders();
            if(Mix_Playing(1) != 1){
                Mix_PlayChannel( 1, snd_manager->mLow, 0);
            }
        }
        
        // スプライトシート上の位置を決定する
        int all_frames = WALKING_ANIM_DISPFRAME * WALKING_ANIM_CNT;
        mSheetAnimIndex = (frame % all_frames)/WALKING_ANIM_DISPFRAME ;
        
        // パーティクル生成、再生成
        for( int i = 0; i < TOTAL_PARTICLES; ++i )
        {
            //Delete and replace dead mParticles
            if( mParticles[ i ] == NULL || mParticles[ i ]->isDead() )
            {
                delete mParticles[ i ];
                mParticles[ i ] = new Particle( mPosX, mPosY );
            }
        }
        
    }
    
    void Player::shiftColliders(){
        for( int i = 0; i < mColliders.size(); i++ )
        {
            mColliders[i].x = mPosX + collisions[i].x;
            mColliders[i].y = mPosY + collisions[i].y;
        }
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
    
    std::vector<SDL_Rect>& Player::getColliders()
    {
        return mColliders;
    }
    
    int Player::getRed(){
        return mR;
    }
    
    int Player::getGreen(){
        return mG;
    }
    
    int Player::getBlue(){
        return mB;
    }
    
    int Player::getAlpha(){
        return mA;
    }
    
    int Player::getSpriteSheetIndex(){
        return mSpriteSheetIndex;
    }
    
    int Player::getSpriteAnimIndex(){
        return mSheetAnimIndex;
    }
    
    int Player::getDir(){
        return mDir;
    }
    
    std::vector<Particle*>& Player::getParticles()
    {
        return mParticles;
    }
    
}
