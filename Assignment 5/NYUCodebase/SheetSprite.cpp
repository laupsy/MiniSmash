//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/19/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "SheetSprite.h"


SheetSprite::SheetSprite(GLuint textureID, unsigned int x, unsigned int y, unsigned int index):
    textureID(textureID),x(x),y(x),index(index){}

void SheetSprite::Draw(float width, float height, float xSprite, float ySprite, float rotation) {
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    
    float u = (float)(((int)index) % x) / (float)x;
    float v = (float)(((int)index) / x) / (float)y;
    float spriteWidth = 1.0f / (float)x;
    float spriteHeight = 1.0f / (float)y;
    
    GLfloat quad[] = { -width * 0.5f,
                        height * 0.5f,
                        -width * 0.5f,
                        -height * 0.5f,
                        width * 0.5f,
                        -height * 0.5f,
                        width * 0.5f,
                        height * 0.5f };
    
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat quadUVs[] = { u, v, u,
                          v + spriteHeight,
                          u + spriteWidth,
                          v + spriteHeight,
                          u + spriteWidth, v };
    
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
}

GLuint SheetSprite::LoadTexture(const char * img_path) {
    
    SDL_Surface *surface = IMG_Load(img_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    SDL_FreeSurface(surface);
    
    return textureID;
    
}