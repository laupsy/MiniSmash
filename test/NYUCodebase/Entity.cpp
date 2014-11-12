//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/19/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

using namespace std;

Entity::Entity(GLuint textureID, float u, float v, float x, float y):
textureID(textureID),u(u),v(v),x(x),y(y) {
    width = SIZE;
    height = SIZE;
    velocity_x = VELOCITY_X;
    velocity_y = VELOCITY_Y;
    acceleration_x = ACCELERATION_X;
    acceleration_y = ACCELERATION_Y;
    friction_x = FRICTION_X;
    friction_y = FRICTION_Y;
    collidesLeft = false;
    collidesRight = false;
    collidesTop = false;
    collidesBottom = false;
    isStatic = false;
}

float Entity::lerp(float v0, float v1, float t) {
    // used with natural movement
    return (1.0-t)*v0 + t*v1;
}

Entity::~Entity() {
    delete this;
}

void Entity::Go() {
    // x movement
    velocity_x = Entity::lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
    velocity_x += acceleration_x * FIXED_TIMESTEP;
    x += velocity_x * FIXED_TIMESTEP;
    // y movement
    velocity_y = Entity::lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
    velocity_y += acceleration_y * FIXED_TIMESTEP;
    y += velocity_y * FIXED_TIMESTEP;
}

//void Entity::MakeLivingEntity() {
//    isStatic = false;
//}

void Entity::Draw(float scale) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    
    GLfloat quad[] = { -width*scale, height*scale, -width*scale, -height*scale, width*scale, -height*scale,
        width*scale, height*scale };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat uvs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
    glTexCoordPointer(2, GL_FLOAT, 0, uvs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
    // POP
    
    glPopMatrix();
}

