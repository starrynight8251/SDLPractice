//
//  main.cpp
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#pragma execution_character_set("utf-8")

#include "sdlmanager.h"

int main( int argc, char* args[] )
{
    // ゲーム管理用クラスを起動
    mygame::SDLManager* sdl_manager = &mygame::SDLManager::getInstance();
    if( !sdl_manager->init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        if( !sdl_manager->loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            sdl_manager->mainloop();
        }
    }
    sdl_manager->cleanup();
    return 0;
}
