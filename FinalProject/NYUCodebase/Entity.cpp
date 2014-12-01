//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/19/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Entity.h"

using namespace std;

Entity::Entity(GLuint textureID, float u, float v, float x, float y, bool floating):
textureID(textureID),u(u+0.0009),v(v+0.0009),x(x),y(y), floating(floating) {
    width = TILEWIDTH * 0.98;
    height = TILEHEIGHT * 0.98;
    velocity_x = VELOCITY_X;
    velocity_y = VELOCITY_Y;
    acceleration_x = ACCELERATION_X;
    acceleration_y = ACCELERATION_Y;
    friction_x = FRICTION_X;
    friction_y = FRICTION_Y;
    collidesLeft = false;
    collidesRight = false;
    collidesTop = false;
    collidesBottom = false;
    isStatic = false;
    solid = true;
}

float Entity::lerp(float v0, float v1, float t) {
    // used with natural movement
    return (1.0-t)*v0 + t*v1;
}

Entity::~Entity() {
    delete this;
}

void Entity::Go(int originPt) {
    
    // reset acceleration from floating
    acceleration_x = ACCELERATION_X;
    acceleration_y = ACCELERATION_Y;
    
    // x movement
    if ( velocity_x != 0.0 ) {
        velocity_x = Entity::lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
        velocity_x += acceleration_x * FIXED_TIMESTEP;
        x += velocity_x * FIXED_TIMESTEP;
    }
    
    // y movement
    if ( !collidesBottom ) {
        velocity_y = Entity::lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
        velocity_y += acceleration_y * FIXED_TIMESTEP;
        y += velocity_y * FIXED_TIMESTEP;
    }
    
    // force float
    if ( y < originPt - 1.33 && player1 ) {
        floating = true;
        velocity_y = -0.15f;
        y = originPt + DEFAULT_Y;
    }
}

void Entity::Float(int originPt) {
    
    // should move slower on x axis when floating
    if ( velocity_y >= 1.2f ) {
        velocity_y = 1.15f;
        acceleration_y = -0.05f;
        acceleration_x = -0.1f;
        velocity_y = Entity::lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
    }
        
    else if ( velocity_y < 0.8f ) {
        velocity_y = 0.85f;
        acceleration_y = 0.5f;
        acceleration_x = 0.1f;
    }
        
    velocity_x = Entity::lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);

    
    if ( !collidesBottom ) {
        velocity_y += acceleration_y * FIXED_TIMESTEP;
        y += velocity_y * FIXED_TIMESTEP;
        velocity_x += acceleration_y * FIXED_TIMESTEP;
        x += velocity_x * FIXED_TIMESTEP;
    }
}

void Entity::Flutter() {
    
    // (rand() % 6 * -1)
    
    if ( velocity_y >= 0.1f ) {
        acceleration_y = -0.5f;
        acceleration_x = (rand() % 6 * -1);
        velocity_y = Entity::lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
    }
    
    else if ( velocity_y < -0.6f ) {
        acceleration_x = (rand() % 6 );
        acceleration_y = 0.5f;
    }
    
    velocity_x = Entity::lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
    velocity_y += acceleration_y * FIXED_TIMESTEP;
    y += velocity_y * FIXED_TIMESTEP;
    velocity_x += acceleration_y * FIXED_TIMESTEP;
    x += velocity_x * FIXED_TIMESTEP;
}

void Entity::FlutterLeft() {
    
    if ( velocity_y >= 0.1f ) {
        acceleration_y = -0.5f;
        acceleration_x = (rand() % 6 * -1);
        velocity_y = Entity::lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
    }
    
    else if ( velocity_y < -0.6f ) {
        acceleration_x = (rand() % 6 );
        acceleration_y = 0.5f;
    }
    
    velocity_x = Entity::lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
    velocity_y += acceleration_y * FIXED_TIMESTEP;
    y += velocity_y * FIXED_TIMESTEP;
    velocity_x += acceleration_y * FIXED_TIMESTEP;
    x -= velocity_x * FIXED_TIMESTEP;
}

void Entity::collidesWith(Entity * e) {
    
    float x_penetration, y_penetration;
    float x_distance, y_distance;
    
    // get y edges
    float thisTopEdge = y + height/2;
    float thisBotEdge = y - height/2;
    float elseTopEdge = e->y + e->height/2;
    float elseBotEdge = e->y - e->height/2;
    
    // get x edges
    float thisLeftEdge = x - width/2;
    float thisRightEdge = x + width/2;
    float elseLeftEdge = e->x - e->width/2;
    float elseRightEdge = e->x + e->width/2;
    
    // check y collision
    // first, is it possible to collide?
    if ( e->solid && thisRightEdge > elseLeftEdge && thisLeftEdge < elseRightEdge ) {
        // next, is it colliding?
        if ( thisTopEdge > elseBotEdge && thisTopEdge < elseTopEdge )
            collidesTop = true;
        if ( thisBotEdge < elseTopEdge && thisBotEdge > elseBotEdge)
            collidesBottom = true;
    }
    
    // check x collision
    // first, is it possible to collide?
    if ( e->solid && thisTopEdge > elseBotEdge && thisBotEdge < elseTopEdge ) {
        // next, is it colliding?
        if ( thisRightEdge > elseLeftEdge && thisRightEdge < elseRightEdge )
            collidesRight = true;
        if ( thisLeftEdge < elseRightEdge && thisLeftEdge > elseLeftEdge)
            collidesLeft = true;
    }
    
    // react to collision
    
    x_distance = fabs(x - e->x);
    y_distance = fabs(y - e->y);
    
    x_penetration = fabs(x_distance - width/2 - e->width/2);
    y_penetration = fabs(y_distance - width/2 - e->width/2);
    
    if ( collidesBottom ) {
        y += y_penetration;
        floating = true;
    }
    else if ( collidesTop )
        y -= y_penetration;
    else if ( collidesLeft )
        x += x_penetration;
    else if ( collidesRight )
        x -= x_penetration;
}

void Entity::Draw(float scale) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    
    glTranslatef(x, y, 0.0);
    
    GLfloat quad[] = { -width*scale, height*scale, -width*scale, -height*scale, width*scale, -height*scale,
        width*scale, height*scale };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat uvs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
    glTexCoordPointer(2, GL_FLOAT, 0, uvs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
    
    // POP
    
    glPopMatrix();
}

