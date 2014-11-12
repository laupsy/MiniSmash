
#include "Game.h"

int main(int argc, char *argv[]) {
    Game g;
    while(!g.UpdateAndRender()) {}
    return 0;
}