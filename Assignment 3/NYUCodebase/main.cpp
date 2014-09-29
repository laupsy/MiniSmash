#include <iostream>
#include "SpaceInvaders.h"
using namespace std;

int main(int argc, char *argv[]) {
    
    SpaceInvaders app;
    while(!app.UpdateAndRender()) {}
    return 0;
    
}