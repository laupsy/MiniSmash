//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/3/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include "Game.h"

Entity::Entity(SheetSprite sprite, float width, float height, float x, float y):
    sprite(sprite), x(x), y(y), width(width), height(height) {
        
        rotation = 0.0f;
        
    }

void Entity::Draw() {

    sprite.Draw(width, height, x, y, rotation);
    
}

void Entity::Update(float elapsed) {
    
}

void Entity::FixedUpdate() {
    
}