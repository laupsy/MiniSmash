#include "Entity.h"

Entity::Entity ( float x, float y, float widtht, float heightt, GLuint texture ) {
    
    posX = x;
    posY = y;
    width = widtht;
    height = heightt;
    rotation = 0.0f;
    textureID = texture;
    
}

void Entity::Draw() {
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(posX, posY, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    
    GLfloat quad[] = { width*-0.5f , height*0.5f , width*-0.5f, height*-0.5f, width*0.5f, height*-0.5f, width*0.5f, height*0.5f };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
}

// Setters

void Entity::setDirection ( float newXDir, float newYDir ) {
    directionX = newXDir;
    directionY = newYDir;
}

void Entity::resetLoc() {
    posY = 0.0;
    posX = 0.0;
}

void Entity::setSpeed ( float newSpeed ) {
    speed = newSpeed;
}

void Entity::setX ( float newX ) {
    posX = newX;
}

void Entity::setY ( float newY ) {
    posY = newY;
}

void Entity::setTexture ( GLuint newTexture ) {
    textureID = newTexture;
}

// Getters

float Entity::getX() { return posX; }
float Entity::getY() { return posY; }

float Entity::getSpeed() { return speed; }

float Entity::getWidth() { return width; }
float Entity::getHeight() { return height; }

float Entity::getDirectionX() { return directionX; }
float Entity::getDirectionY() { return directionY; }

