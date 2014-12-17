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

#include <stdio.h>
#include <Math.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define OFFSET 0.01f
#define TILEWIDTH 72.0f/990.0f
#define TILEHEIGHT 72.0f/749.0f
#define VELOCITY_X 3.0f
#define VELOCITY_Y 2.0f
#define ACCELERATION_X 1.5f
#define ACCELERATION_Y -9.8f // gravity
#define FRICTION_X 0.99999f
#define FRICTION_Y 0.99999f
#define SCALE 0.7f
#define DEFAULT_Y 1.0f
#define LEVELWIDTH 10
#define SNOW_TRANSITION 40.0
#define SPACE_TRANSITION 70.0
#define END_TRANSITION 85.0

#define BLOCKS 100
#define RAINDROPS 100

#define FONT_SIZE 0.06f
#define FONT_SPACING 0.0005f


class Entity {
public:
    Entity(GLuint textureID, float u, float v, float x, float y, bool floating = false);
    ~Entity();
    
    void Draw(float scale);
    void FixedUpdate();
    void Go(int originPt);
    void KO(int originPt);
    void Float(int originPt);
    void Flutter();
    void FlutterLeft();
    void ShootProjectile(Entity * e, Entity * player);
    
    bool collidesWith(Entity * e);
    
    float lerp(float v0, float v1, float t);
    
    float u, v;
    float x, y;
    float width, height;
    float dir_x, dir_y;
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float friction_x, friction_y;
    
    float jumpPoint;
    
    bool collidesLeft, collidesRight, collidesTop, collidesBottom;
    bool solid;
    bool isStatic;
    bool floating;
    bool shaking;
    bool jumping;
    bool player1;
    bool player2;
    bool notShooting;
    bool hit;
    
    int lastDirection;
    int damage;
    int score;
    float onHitDamage;
    
    GLuint textureID;
};