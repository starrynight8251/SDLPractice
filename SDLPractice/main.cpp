#include <SDL2/SDL.h>

int main(int, char ** const)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Hey",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,0);
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0);
    
    
    SDL_Event ev;
    while(true){
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        while(SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                return 0;
        }
        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        SDL_RenderDrawLine(render,10, 10, 400, 400);
        SDL_RenderPresent(render);
    }
    
    return 0;
}
