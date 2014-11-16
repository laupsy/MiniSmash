//
//  Shape.h
//  NYUCodebase
//
//  Created by Laura Barbera on 11/14/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Shape__
#define __NYUCodebase__Shape__

#include <stdio.h>

class Shape {
public:
    Shape(float x, float y, bool solid);
    ~Shape();
    
    void Draw(float scale);
    void Go();
    
    float x,y;
    float width, height;
    float dir_x, dir_y;
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float friction_x, friction_y;
    
    bool collidesLeft, collidesRight, collidesTop, collidesBottom;
    bool solid;
};

#endif /* defined(__NYUCodebase__Shape__) */
