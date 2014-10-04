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
    
    float width;
    float height;
    float x;
    float y;
    float u;
    float v;
    float speed;
    float direction;
    float scale;
    
    GLuint textureID;
    
    bool visible;
    bool targetable;
    
};

#endif /* defined(__NYUCodebase__Entity__) */
