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

    Entity(unsigned int textureID, float x, float y, float index);
    
    void Draw(float scale);
    
    float x;
    float y;
    float index;
    float width;
    float height;
    
    bool visible;
    bool solid;
    
    GLuint textureID;
};

#endif /* defined(__NYUCodebase__Entity__) */
