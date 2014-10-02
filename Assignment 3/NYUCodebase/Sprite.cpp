#include "Sprite.h"
#include <iostream>

Sprite::Sprite(float xPos, float yPos, float uVal, float vVal, float swidth, float sheight, bool visibility, GLuint spriteTexture) {
    x = xPos;
    y = yPos;
    u = uVal;
    v = vVal;
    width = swidth;
    height = sheight;
    visible = visibility;
    texture = spriteTexture;
}

void Sprite::drawSprite(float scale) {
    
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    
    GLfloat quad[] = { width*-1.0f*scale, height*scale, width*-1.0f*scale, height*-1.0f*scale, width*scale, height*-1.0f*scale, width*scale, height*scale };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat quadUVs[] = { u, v, u, v+height, u+width, v+height, u+width, v };
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
}

float Sprite::getX() {
    return x;
}

float Sprite::getY() {
    return y;
}

float Sprite::getU() {
    return u;
}

float Sprite::getV() {
    return v;
}

float Sprite::getWidth() {
    return width;
}

float Sprite::getHeight() {
    return height;
}

void Sprite::setX ( float newX ) {
    x = newX;
}

void Sprite::setY ( float newY ) {
    y = newY;
}

void Sprite::setU ( float newU ) {
    u = newU;
}

void Sprite::setV ( float newV ) {
    v = newV;
}