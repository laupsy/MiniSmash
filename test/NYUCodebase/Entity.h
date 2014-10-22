
#include <stdio.h>
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <Math.h>

#define LEVEL_HEIGHT 16
#define LEVEL_WIDTH 22
#define SPRITE_COUNT_X 10
#define SPRITE_COUNT_Y 1
#define TILE_SIZE 0.2f
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define OFFSET 0.001f
#define JUMP_HEIGHT 50.0f

class Entity {
public:
    Entity(GLuint textureID, float u, float v, float x, float y, bool isStatic);
    ~Entity();
    
    void Reset();
    void Draw(float scale);
    void FixedUpdate();
    void MoveHoriz();
    void Fall();
    void Jump();
    bool CheckCollision(Entity * e);
    
    float lerp(float v0, float v1, float t);
    
    float u,v,x,y,width,height,dir_x,dir_y;
    float velocity_x,velocity_y,speed,acceleration_x,acceleration_y,friction_x,friction_y;
    GLuint textureID;
    bool isStatic,collidesLeft,collidesRight,collidesTop,collidesBottom,jumping;
};