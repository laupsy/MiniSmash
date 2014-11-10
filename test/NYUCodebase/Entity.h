
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

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define OFFSET 0.001f

#define SIZE  36.0f/360.0f
#define VELOCITY_X 7.0f
#define VELOCITY_Y 7.0f
#define ACCELERATION_X 0.1f
#define ACCELERATION_Y -5.0f
#define FRICTION_X 0.99999f
#define FRICTION_Y 0.99999f

class Entity {
public:
    Entity(GLuint textureID, float u, float v, float x, float y);
    ~Entity();
    
    void Draw(float scale);
    void FixedUpdate();
    void Go();
    void Stop();
    void MakeLivingEntity();
    void ResetX();
    void ResetY();
    
    float lerp(float v0, float v1, float t);
    
    float u, v;
    float x, y;
    float width, height;
    float dir_x, dir_y;
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float friction_x, friction_y;
    
    bool collidesLeft, collidesRight, collidesTop, collidesBottom;
    bool isStatic;
    
    GLuint textureID;
};