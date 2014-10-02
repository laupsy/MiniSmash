//
//  Bullet.h
//  NYUCodebase
//
//  Created by Laura Barbera on 10/1/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Bullet__
#define __NYUCodebase__Bullet__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

#include <stdio.h>

class Bullet {
public:
    Bullet();
    Bullet(float width, float height, float x, float y);
    void display();
    void shoot(float dir, float elapsed);
    
    float direction;
    float x;
    float y;
    float width;
    float height;
    float speed;
};

#endif /* defined(__NYUCodebase__Bullet__) */
