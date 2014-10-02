#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <math.h>

class Sprite {
public:
    Sprite( float x, float y, float u, float v, float width, float height, bool visible, GLuint texture );
    void drawSprite(float scale);
    
    float getX();
    float getY();
    float getU();
    float getV();
    float getWidth();
    float getHeight();
    
    void setX ( float newX );
    void setY ( float newY );
    void setU ( float newU );
    void setV ( float newV );
    
    float speed = 0.01;
    bool visible;
    GLuint texture;
    
private:
    float x;
    float y;
    float u;
    float v;
    float width;
    float height;
    
    
};