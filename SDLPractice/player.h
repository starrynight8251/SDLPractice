//
//  player.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/08.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef player_h
#define player_h

#include <SDL2/SDL.h>

//The player that will move around on the screen
class Player
{
public:
	static const int WALKING_ANIMATION_FRAMES = 4;
    //The dimensions of the player
    static const int PLAYER_X      = 0;
    static const int PLAYER_Y      = 0;
    static const int PLAYER_WIDTH  = 32;
    static const int PLAYER_HEIGHT = 32;
    
    static const int PLAYER_COLLIDE_X =  8;
    static const int PLAYER_COLLIDE_Y =  0;
    static const int PLAYER_COLLIDE_W = 16;
    static const int PLAYER_COLLIDE_H = 32;
    
    //Maximum axis velocity of the player
    static const int PLAYER_VEL = 8;
    enum Dir{
        LEFT  = 3,
        RIGHT = 1,
        UP    = 0,
        DOWN  = 2,
    };

    //static int dir_off[] = { 3, 1, 0, 2 };
	    
	//Initializes the variables
    Player();
    ~Player(); 
    //Takes key presses and adjusts the player's velocity
    void handleEvent( SDL_Event& e );
    
    //Moves the player
    void move( SDL_Rect& wall );
    
    //Shows the player on the screen
    void render( int frame );
    
private:
	// キャラクタ
	LTexture* mPlayerTexture;
	SDL_Rect  mPlayerClips[ WALKING_ANIMATION_FRAMES ];
	
    //The X and Y offsets of the player
    int mPosX, mPosY;
    Dir mDir;
    
    //The velocity of the player
    int mVelX, mVelY;

    //Player's collision box
    SDL_Rect mCollider;

};


#endif /* player_h */
