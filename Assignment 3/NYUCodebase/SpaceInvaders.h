#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <algorithm>
#include "Sprite.h"
using namespace std;

#define MAX_BULLETS 100

enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_PAUSE, STATE_GAME_OVER };

class SpaceInvaders {
public:
    SpaceInvaders();
    
    void Init();
    bool Update();
    void Render();
    
    void renderMain();
    void renderPause();
    void renderLevel();
    void renderEnd();
    void dropBone();
    void isColliding();
    
    void inputActions();
    void behaviorActions();
    void loadEntities();
    
    vector<Sprite> barnAliens;
    
    GLuint loadTexture(const char *image_path);
    void drawText(unsigned textTexture, string text, float x, float y, float size, float r, float g, float b, float a);
    void shootBullet( Sprite obj);
    
    GLuint textSheet;
    const Uint8 *keys;
    
    
private:
    bool done;
    int state;
    int movedirection;
    int bulletIndex;
    int score;
    int cooldown = 0;
    int dropBuffer = 0;
    
    vector<Sprite> bullets;
    vector<Sprite> zeldas;
    
    SDL_Window* displayWindow;
};