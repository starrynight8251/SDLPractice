//
//  person.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/23.
//  Copyright © 2017年 NoCompany. All rights reserved.
//
#include "person.h"
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include "helper.h"
#include "gamemanager.h"
#include "graphicmanager.h"
#include "soundmanager.h"

namespace mygame {
Person::Person( int idx, std::vector<Tile*>& tiles )
{
    GraphicManager* grp_manager = &GraphicManager::getInstance();
    
    mPosX = Person_X;
    mPosY = Person_Y;
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
    
    while(true){
        mPosX = ( rand() % GraphicManager::SCREEN_WIDTH );
        mPosY = ( rand() % GraphicManager::SCREEN_HEIGHT );
        shiftColliders();
        if( !touchesWall(mColliders, tiles) ){
            break;
        }
    }
    
    mPersonTexture = grp_manager->gPersonTexture;
    int offset_x = 96*(idx%5);
    int offset_y = 128*(idx/5);
    
    mPersonClips[ 0 ].x =   0 + offset_x;
    mPersonClips[ 0 ].y =   0 + offset_y;
    mPersonClips[ 0 ].w =  32;
    mPersonClips[ 0 ].h =  32;
    
    mPersonClips[ 1 ].x =  32 + offset_x;
    mPersonClips[ 1 ].y =   0 + offset_y;
    mPersonClips[ 1 ].w =  32;
    mPersonClips[ 1 ].h =  32;
    
    mPersonClips[ 2 ].x =  64 + offset_x;
    mPersonClips[ 2 ].y =   0 + offset_y;
    mPersonClips[ 2 ].w =  32;
    mPersonClips[ 2 ].h =  32;
    
    mPersonClips[ 3 ].x =  32 + offset_x;
    mPersonClips[ 3 ].y =   0 + offset_y;
    mPersonClips[ 3 ].w =  32;
    mPersonClips[ 3 ].h =  32;
    
    
    // パーティクル用領域確保
    particles.resize( GraphicManager::TOTAL_PARTICLES );
}

Person::~Person()
{
    mPersonTexture->free();
    mPersonTexture = NULL;
    // パーティクル解放
    for( int i = 0; i < GraphicManager::TOTAL_PARTICLES; ++i )
    {
        delete particles[ i ];
    }
    particles.clear();
}

void Person::move( int frame, std::vector<Tile*>& tiles )
{
    if( frame % 4 == 0){
        mDir = (Dir)(rand() % 4);
        if(mDir == LEFT){
            mVelX = -Person_VEL;
            mVelY = 0;
        }
        if(mDir == RIGHT){
            mVelX = Person_VEL;
            mVelY = 0;
        }
        if(mDir == UP){
            mVelX = 0;
            mVelY = -Person_VEL;
        }
        if(mDir == DOWN){
            mVelX = 0;
            mVelY = Person_VEL;
        }
    }

    // 移動処理　X
    mPosX += mVelX;
    shiftColliders();
    
    // ウィンドウ境界と壁との当たり判定　X
    if( ( mPosX < 0 ) || ( mPosX + Person_WIDTH > GraphicManager::LEVEL_WIDTH ) || touchesWall( mColliders, tiles ) )
    {
        // 壁に入ってしまうので戻す
        mPosX -= mVelX;
        shiftColliders();
    }
    
    // 移動処理 Y
    mPosY += mVelY;
    shiftColliders();
    
    // ウィンドウ境界と壁との当たり判定　Y
    if( ( mPosY < 0 ) || ( mPosY + Person_HEIGHT > GraphicManager::LEVEL_HEIGHT ) || touchesWall( mColliders, tiles ) )
    {
        // 壁に入ってしまうので戻す
        mPosY -= mVelY;
        shiftColliders();
    }
    
    if(frame % 4 == 3){
        mVelX = 0;
        mVelY = 0;
    }
}

void Person::renderParticles(SDL_Rect& camera){
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

void Person::setCamera( SDL_Rect& camera )
{
    // カメラ位置設定
    camera.x = ( mPosX + Person_WIDTH / 2 ) - GraphicManager::SCREEN_WIDTH / 2;
    camera.y = ( mPosY + Person_HEIGHT / 2 ) - GraphicManager::SCREEN_HEIGHT / 2;
    
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

void Person::render( int frame, SDL_Rect& camera )
{
    // プレイヤーの表示
    int all_frames = WALKING_ANIM_DISPFRAME * WALKING_ANIM_CNT;
    SDL_Rect currentClip = mPersonClips[ (frame % all_frames)/WALKING_ANIM_DISPFRAME ];
    currentClip.y += mDir*currentClip.h;
    mPersonTexture->render( mPosX-camera.x, mPosY-camera.y, &currentClip, mDegrees, NULL, mFlipType );
    renderParticles(camera);
}

int Person::getPosX(){
    return mPosX;
}

int Person::getPosY(){
    return mPosY;
}

void Person::setPosX(int posX){
    mPosX = posX;
}

void Person::setPosY(int posY){
    mPosY = posY;
}

void Person::shiftColliders(){
    for( int i = 0; i < mColliders.size(); i++ )
    {
        mColliders[i].x = mPosX + collisions[i].x;
        mColliders[i].y = mPosY + collisions[i].y;
    }
}

std::vector<SDL_Rect>& Person::getColliders()
{
    return mColliders;
}

}
