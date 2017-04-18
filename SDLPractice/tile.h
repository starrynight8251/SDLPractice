//
//  tile.h
//  SDLPractice
//
//  Created by starrynight on 2017/04/17.
//  Copyright © 2017年 NoCompany. All rights reserved.
//

#ifndef tile_h
#define tile_h

class Tile
{
public:
    //Initializes position and type
    Tile( int x, int y, int tileType );
    
    //Shows the tile
    void render( SDL_Rect& camera );
    
    //Get the tile type
    int getType();
    
    //Get the collision box
    SDL_Rect getBox();
    
private:
    //The attributes of the tile
    SDL_Rect mBox;
    
    //The tile type
    int mType;
};


#endif /* tile_h */
