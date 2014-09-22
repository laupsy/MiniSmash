#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

// These are the objects in pong that will respond to input

class Entity {
    
private:
    
    float posX, posY, rotation, scale, width, height, speed, directionX, directionY;
    
    GLuint textureID;
    
public:
    
    Entity ( float x, float y, float width, float height, GLuint texture );
    
    void Draw();
    
    // setters
    
    void resetLoc(); // called when ball goes off screen
    void setSpeed( float newSpeed );
    void setDirection( float newXDir, float newYDir );
    void setX( float newX );
    void setY( float newY );
    void setTexture( GLuint newTexture );
    
    // getters
    
    float getX();
    float getY();
    float getSpeed();
    float getWidth();
    float getHeight();
    float getDirectionX();
    float getDirectionY();
    
};