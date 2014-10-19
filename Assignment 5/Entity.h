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
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

#include "SheetSprite.h"

class Entity {
    
public:

    Entity(SheetSprite sprite, float x, float y, float width, float height);
    
    void Draw();
    void Update(float elapsed);
    void FixedUpdate();
    
    float x;
    float y;
    float index;
    float width;
    float height;
    float rotation;
    
    bool visible;
    bool solid;
    
    GLuint textureID;
    SheetSprite sprite;
};

#endif /* defined(__NYUCodebase__Entity__) */
