//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 9/29/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "SheetSprite.h"

void SheetSprite::Draw(float scale) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLfloat quad[] = {-width * scale , height * scale, -width * scale, -height * scale, width * scale, -height * scale, width * scale, height * scale};
    GLfloat quadUVs[] = {u, v, u, v+height, u+width, v+height, u+width, v};
    // draw arrays
}