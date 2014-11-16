//
//  Shape.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 11/14/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Shape.h"
#include "Entity.h"

Shape::Shape(float x, float y, bool solid) : x(x), y(y), solid(solid) {}

Shape::~Shape() {
    delete this;
}

void Shape::Draw(float scale) {
    GLfloat shape[] = {x*scale,y*scale,(x+1)*scale,(y+1)*scale};
    glVertexPointer(2, GL_FLOAT, 0, shape);
    glDrawArrays(GL_LINES, 0, 3);
}

void Shape::Go() {
    
}