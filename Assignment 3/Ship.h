//
//  Ship.h
//  NYUCodebase
//
//  Created by Laura Barbera on 9/28/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Ship__
#define __NYUCodebase__Ship__

#include <stdio.h>
#include "Bullet.h"
#include "Ship.h"

class Ship {
public:
    // call on game start
    Ship();
    // call when game over - losing life only decreases life count, does not remove current ship or create new ship
    ~Ship();
    
    // Ship functions:  lose life, move left/right, collide
    
    void moveLeft();
    void moveRight();
    
    bool collision(const Ship& e1, const Bullet& e2);
    
private:
    bool isEnemy;
};

#endif /* defined(__NYUCodebase__Ship__) */
