//
//  Entity.h
//  NYUCodebase
//
//  Created by Laura Barbera on 10/3/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Entity__
#define __NYUCodebase__Entity__

#include <stdio.h>

class Entity {
    
public:

    Entity(unsigned int textureID, float x, float y, float u, float v, float width, float height);
//    ~Entity();
    
    void Draw(float scale);
    void setX(const float& newX);
    void setY(const float& newY);
    void setCoords(const float& newX, const float& newY);
    
    void moveLeft();
    void constantLeft();
    void moveRight();
    void constantRight();
    void moveUp();
    void moveDown();
    
    void fall();
    
    void resetPhysics();
    
    void jump();
    
    void offScreen();
    
    void checkVisibility();
    
    bool onGround();
    
    float width;
    float height;
    float x;
    float y;
    float u;
    float v;
    float scale;
    
    float edgeLeft = x + width / 2.0f;
    float edgeRight = x - width / 2.0f;
    float edgeTop = y + width / 2.0f;
    float edgeBottom = y - width / 2.0f;
    
    float speed = 0.01f;
    float speed_x = 0.020f;
    float speed_y = 0.015f;
    float maxSpeed = 0.035f;
    
    float direction_x = 0.0f;
    float direction_y = 0.0f;
    
    float acceleration_x = 1.029f;
    float acceleration_y = 1.0098f;
    
    float friction_x = 0.98f;
    float friction_y = 0.89f;
    
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;
    
    float jumpSpeed = 0.2f;
    
    GLuint textureID;
    
    bool visible = true;
    bool targetable = false;
    bool jumping = false;
    bool atLadder = false;
    bool floating = true;
    bool solid = true;
    
};

#endif /* defined(__NYUCodebase__Entity__) */
