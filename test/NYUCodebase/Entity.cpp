//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/19/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

using namespace std;

float Entity::lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

Entity::Entity(GLuint textureID, float u, float v, float x, float y, bool isStatic):
textureID(textureID),u(u),v(v),x(x),y(y),isStatic(isStatic) {
    width = 36.0f/360.0f;
    height = 36.0f/360.0f;
    speed = VELOCITY;
    velocity_x = speed;
    velocity_y = speed;
    acceleration_x = ACCELERATION;
    acceleration_y = ACCELERATION;
    friction_x = FRICTION;
    friction_y = FRICTION;
    dir_x = 1.0f;
    dir_y = 1.0f;
    collidesLeft = false;
    collidesRight = false;
    collidesTop = false;
    collidesBottom = false;
    jumping = false;
}

void Entity::Reset() {
    speed = VELOCITY;
    velocity_x = speed;
    velocity_y = speed;
    acceleration_x = ACCELERATION;
    acceleration_y = ACCELERATION;
    friction_x = FRICTION;
    friction_y = FRICTION;
}

Entity::~Entity() {
    delete this;
}

void Entity::MoveHoriz() {
    velocity_x += ACCELERATION * FIXED_TIMESTEP;
    x += velocity_x * FIXED_TIMESTEP * dir_x;
}

void Entity::Fall(float elapsed) {
    velocity_y += acceleration_y * FIXED_TIMESTEP;
    y -= velocity_y * FIXED_TIMESTEP;
}

void Entity::Jump() {
    if (!jumping) {
        velocity_y = JUMP_HEIGHT + lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_x);
        y += ( velocity_y * FIXED_TIMESTEP ) * dir_y;
//        velocity_x = JUMP_HEIGHT + lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
//        x += ( velocity_x * FIXED_TIMESTEP ) * dir_x;
    }
}

bool Entity::CheckCollision(Entity * e) {
    
    bool leftLargerThanRight = x - width/2.0f > e->x + e->width/2.0f;
    bool rightSmallerThanLeft = x + width/2.0f < e->x - e->width/2.0f;
    bool bottomHigherThanTop = y - height/2.0f > e->y + e->height/2.0f;
    bool topLowerThanBottom = y + height/2.0f < e->y - e->height/2.0f;
    
    if ( leftLargerThanRight ) return false;
    if ( rightSmallerThanLeft ) return false;
    if ( bottomHigherThanTop ) return false;
    if ( topLowerThanBottom ) return false;
    
    return true;
}

void Entity::FixedUpdate() {}

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
    GLfloat uvs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
    glTexCoordPointer(2, GL_FLOAT, 0, uvs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}
