#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

// These are the objects in pong that will respond to input

class Entity {
    
private:
    
public:
    
    Entity ( float x, float y, float width, float height, GLuint texture );
    
    void Draw();
    
    void resetLoc();
    void setSpeed( float newSpeed );
    void setDirection( float newXDir, float newYDir );
    void setX( float newX );
    void setY( float newY );
    void setTexture( GLuint newTexture );
    
    float x;
    float y;
    float scale;
    float width;
    float height;
    float speed;
    float directionX;
    float directionY;
    
    float rotation = 0.0f;
    
    GLuint textureID;
    
};