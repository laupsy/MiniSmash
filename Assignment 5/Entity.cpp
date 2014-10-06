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
//    cout << "First x: " << x << "\n";
    x = newX;
//    cout << "New x: " << x << "\n";
}

void Entity::setY(const float& newY) {
    y = newY;
}

void Entity::setCoords(const float& newX, const float& newY) {
    setX(newX);
    setY(newY);
}

//void Entity::moveLeft(float elapsed) {
//    if ( ! ( colliding ) ) {
//        setX(x - elapsed);
//        direction_x = -1.0;
//        direction_y = 0.0;
//    }
//    else if ( direction_x != -1.0 ) {
//        setX(x - elapsed);
//        direction_x = -1.0;
//        direction_y = 0.0;
//        colliding = false;
//    }
//}
//
//void Entity::moveRight(float elapsed) {
//    if ( ! ( colliding ) ) {
//        setX(x + elapsed);
//        direction_x = 1.0;
//        direction_y = 0.0;
//    }
//    else if ( direction_x != 1.0 ) {
//        setX(x + elapsed);
//        direction_x = 1.0;
//        direction_y = 0.0;
//        colliding = false;
//    }
//}

void Entity::moveLeft() {
        if ( speed > 0.001 ) speed = speed * friction_x;
        else speed = 0; // should stop moving when too slow
        setX(x - speed);
        direction_x = -1.0;
        direction_y = 0.0;
        u = width * 1;
}

void Entity::constantLeft() {
        if ( speed < maxSpeed ) {
            speed = speed * acceleration_x;
        }
        setX(x - speed);
        direction_x = -1.0;
        direction_y = 0.0;
        u = width * 1;
}

void Entity::moveRight() {
        if ( speed > 0.001 ) speed = speed * friction_x;
        else speed = 0; // should stop moving when too slow
        setX(x + speed);
        direction_x = 1.0;
        direction_y = 0.0;
        u = width * 3.5;
}

void Entity::constantRight() {
    if ( speed < maxSpeed ) {
            speed = speed * acceleration_x;
        }
        setX(x + speed);
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
        speed = speed * acceleration_y;
        setY(y - speed);
        u = width * 3.5;
    }
}

void Entity::jump() {
    
//        if ( direction_x == 1.0f ) {
//            x += 0.0015f * friction_x;
//            u = width * 3.5;
//        }
//        else {
//            x -= 0.0015f * friction_x;
//            u = width * 1;
//        }
    
        y += 0.04f;
}

void Entity::resetPhysics() {
    
    speed = 0.015f;
    
    acceleration_x = 1.009f;
    acceleration_y = 1.003f;
    
    friction_x = 0.98f;
}

void Entity::offScreen() {
    if ( x > 0.95f ) {
        x = -0.95f;
    }
    else if ( x < -0.95f ) {
        x = 0.95f;
    }
    else if ( y < -0.95f ) {
        resetPhysics();
        y = 0.95f;
    }
}