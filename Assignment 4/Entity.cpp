//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/3/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

#include "Entity.h"
#include "Game.h"

Entity::Entity(unsigned int textureID, float x, float y, float u, float v, float width, float height):
    textureID(textureID), x(x), y(y), u(u), v(v), width(width), height(height) {}

//Entity::~Entity() {
//    delete this;
//}

void Entity::Draw(float scale) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
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

void Entity::setX(const float& newX) {
    x = newX;
}

void Entity::setY(const float& newY) {
    y = newY;
}

void Entity::setCoords(const float& newX, const float& newY) {
    setX(newX);
    setY(newY);
}