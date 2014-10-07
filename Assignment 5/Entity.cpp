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

Entity::Entity(unsigned int textureID, float x, float y, float index):
    textureID(textureID), x(x), y(y), index(index) {
        width = 36.0f/360.0f;
        height = 36.0f/360.0f;
    }

void Entity::Draw(float scale) {
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    
    GLfloat quad[] = { -width*scale, height*scale, -width*scale, -height*scale, width*scale, -height*scale,
        width*scale, height*scale };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    float u = index * width;
    float v = 0.0f;
    
    GLfloat uvs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
    glTexCoordPointer(2, GL_FLOAT, 0, uvs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisableClientState(GL_COLOR_ARRAY);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
}