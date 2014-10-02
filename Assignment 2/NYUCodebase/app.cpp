
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "App.h"

using namespace std;

/* 
 
 Laura Barbera
 CS3113 - assignment3
 github.com/laupsy
 
*/

int main(int argc, char *argv[]) {

    App g;
    
    while (!g.UpdateAndRender()){
    }
    return 0;
}