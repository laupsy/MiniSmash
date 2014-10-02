//
//  Bullet.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/1/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Bullet.h"

Bullet::Bullet():width(0.05f),height(10.0f),x(0.0f),y(0.0f) {}

Bullet::Bullet(float width, float height, float x, float y):
    width(width),height(height),x(x),y(x){
    
    glLoadIdentity();
        
    glTranslatef(x,y, 0.0);
    
    GLfloat quad[] = { -width/height, width, -width/height, -width, width/height, -width, width/height, width };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_QUADS, 0, 4);
    
}

void Bullet::display(float xPos, float yPos) {
    
    glDrawArrays(GL_QUADS, 0, 4);
}

void Bullet::shoot(float dir, float elapsed) {
    direction = dir;
    y += dir * speed * elapsed;
}