//
//  SheetSprite.h
//  NYUCodebase
//
//  Created by Laura Barbera on 10/19/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__SheetSprite__
#define __NYUCodebase__SheetSprite__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

class SheetSprite {
    
public:
    
    SheetSprite(GLuint textureID, unsigned int x, unsigned int y, unsigned int index);
    
    void Draw(float width, float height, float xSprite, float ySprite, float rotation);
    
    GLuint LoadTexture(const char * img_path);
    
    GLuint textureID;
    unsigned int x;
    unsigned int y;
    unsigned int index;
    
};

#endif /* defined(__NYUCodebase__SheetSprite__) */
