//
//  SheetSprite.h
//  NYUCodebase
//
//  Created by Laura Barbera on 9/29/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__SheetSprite__
#define __NYUCodebase__SheetSprite__

#include <stdio.h>

class SheetSprite {
    
public:
    
    SheetSprite();
    SheetSprite(unsigned int textureID, float u, float v, float width, float height);
    
    void Draw(float scale);
    
    float scale,u,v,width,height;
    unsigned int textureID;
    
};

#endif /* defined(__NYUCodebase__SheetSprite__) */
