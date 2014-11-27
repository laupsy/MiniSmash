//
//  World.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 11/26/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "World.h"

World::World() {
    
}

void World::PlaceBlocks() {
    
    float randXLoc, randYLoc;
    
    for ( size_t i = 0; i < BLOCKS; i++ ) {

            if ( ( rand() % 10 ) % 2 == 0 )
                randXLoc = -1.33f;
            else
                randXLoc = -0.1f;
        
            randYLoc =  ( rand() % LEVELWIDTH * 10 - LEVELWIDTH/2 ) / 10.0f; // convert to float
            
            // prevent overlap
            
            if ( blocks.size() > 0 ) {
                Entity * prevEntity = blocks[blocks.size() - 1];
                if ( fabs(prevEntity->y - randYLoc) <= 0.1f )
                    randYLoc += 0.1f;
            }
            
            // row of blocks
            
            for ( int i = 0; i < 5; i++ ) {
                Entity * block = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 0.0f, TILEHEIGHT * 2.0f, randXLoc + TILEWIDTH * i + 0.5, randYLoc);
                blocks.push_back(block);
            }
        }

}

GLuint World::LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void World::DrawText( GLuint textTexture, std::string text, float x, float y, float spacing, float size, float r, float g, float b, float a ) {
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    glTranslatef( x, y, 0.0 );
    std::vector<float> vertexData;
    std::vector<float> textCoordData;
    std::vector<float> colorData;
    float textureSize = 1.0/16.0f;
    for ( int i=0; i <text.size(); i++ ) {
        float textX = (float) (((int)text[i]) % 16 ) / 16.0f;
        float textY = (float) (((int)text[i]) / 16 ) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size});
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        textCoordData.insert(textCoordData.end(),
                             {   textX,
                                 textY,
                                 textX,
                                 textY + textureSize,
                                 textX + textureSize,
                                 textY + textureSize,
                                 textX + textureSize,
                                 textY
                             });
    }
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer (2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, textCoordData.data());
    glEnableClientState( GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}