
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include "Entity.h"


SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
    
    Game g;
    
    while(!g.UpdateAndRender()) {}
    return 0;
    
}