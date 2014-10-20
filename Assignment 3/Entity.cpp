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

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

// Linear Interpolation

float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

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

void Entity::moveLeft() {
    
    if ( speed_x > 0.5 ) speed_x = lerp(speed_x, 0.0f, FIXED_TIMESTEP * friction_x);
    else speed_x = 0;
    setX(x - speed_x * FIXED_TIMESTEP);
    cout << "This is speed: " << speed_y << endl;
    direction_x = -1.0;
    direction_y = 0.0;
    u = width * 1;
}

void Entity::constantLeft() {
//    if ( speed_x < maxSpeed ) {
//        speed_x = speed_x * acceleration_x;
//    }
//    setX(x - speed_x);
//    direction_x = -1.0;
//    direction_y = 0.0;
//    u = width * 1;
}

void Entity::moveRight() {
    if ( speed_x > 0.5 ) speed_x = lerp(speed_x, 0.0f, FIXED_TIMESTEP * friction_x);
    else speed_x = 0;
    setX(x + speed_x * FIXED_TIMESTEP);
    cout << "This is speed: " << speed_x << endl;
    direction_x = 1.0;
    direction_y = 0.0;
    u = width * 3.5;
}

void Entity::constantRight() {
    if ( speed_x < maxSpeed ) {
        speed_x += acceleration_x * FIXED_TIMESTEP;
    }
    
    speed_x = lerp(speed_x, 0.0f, FIXED_TIMESTEP);
    
    setX(x + speed_x * FIXED_TIMESTEP);
    cout << "This is speed: " << speed_x << endl;
    direction_x = 1.0;
    direction_y = 0.0;
    u = width * 3.5;
}

void Entity::moveUp() {
    setY(y + speed);
    direction_y = 1.0;
    direction_x = 0.0;
    u = width * 2.5;
}

void Entity::moveDown() {
    setY(y - speed);
    direction_y = -1.0;
    direction_x = 0.0;
    u = width * 2.5;
}

void Entity::fall() {
    if ( floating ) {
        
        
        
            speed_y = lerp(speed_y, 0.0f, FIXED_TIMESTEP);
            speed_y += acceleration_y * FIXED_TIMESTEP;
            setY(y - speed_y * FIXED_TIMESTEP);
        
        
//        speed = speed * acceleration_y;
//        setY(y - speed);
        u = width * 3.5;
    }
}

void Entity::jump() {
    
//    speed_y = lerp(speed_y, 0.0f, FIXED_TIMESTEP * friction_y);
//    speed_y += acceleration_y * FIXED_TIMESTEP;
//    setY(y + 0.005f +  speed_y * FIXED_TIMESTEP);
    
    setY(y + 0.05f);
}

void Entity::resetPhysics() {
    
    speed = 0.01f;
    speed_x = 2.25f;
    speed_y = 2.25f;
    
    acceleration_x = 3.5f;
    acceleration_y = 3.5f;
    
    friction_x = 0.98f;
    friction_y = 0.89f;
}

void Entity::offScreen() {
    if ( x > 0.95f ) {
        x = -0.95f;
    }
    else if ( x < -0.95f ) {
        x = 0.95f;
    }
}

void Entity::checkVisibility() {
    if ( visible == false ) Draw(0.0f);
}

void Entity::FixedUpdate() {
    
//    velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
//    velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
//    velocity_x += acceleration_x * FIXED_TIMESTEP;
//    velocity_y += acceleration_y * FIXED_TIMESTEP;
//    x += velocity_x * FIXED_TIMESTEP;
//    y += velocity_y * FIXED_TIMESTEP;
}