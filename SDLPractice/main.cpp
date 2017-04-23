#include "gamemanager.h"

int main( int argc, char* args[] ){
    // ゲーム管理用クラスを起動
    mygame::GameManager* gm_manager = &mygame::GameManager::getInstance();
    if( !gm_manager->init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        if( !gm_manager->loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            gm_manager->mainloop();
        }
    }
    gm_manager->cleanup();
    return 0;
}
