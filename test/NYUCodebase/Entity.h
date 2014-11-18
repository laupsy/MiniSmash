
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
#include <SDL_mixer.h>

#include <Math.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define OFFSET 0.01f

#define TILEWIDTH  72.0f/600.0f
#define TILEHEIGHT 72.0f/454.0f
#define VELOCITY_X 1.0f
#define VELOCITY_Y 1.0f
#define ACCELERATION_X 1.5f
#define ACCELERATION_Y -4.0f // gravity
#define FRICTION_X 0.99999f
#define FRICTION_Y 0.99999f
#define SCALE 0.6f

#define LEVELWIDTH 7

class Entity {
public:
    Entity(GLuint textureID, float u, float v, float x, float y);
    ~Entity();
    
    void Draw(float scale);
    void FixedUpdate();
    void Go();
    void Float();
    void MakeLivingEntity();
    void Jump();
    void ResetX();
    void ResetY();
    
    bool IsColliding(std::vector<Entity*> entities);
    
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
    bool floating;
    
    GLuint textureID;
};