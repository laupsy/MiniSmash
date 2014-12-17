//
//  World.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 11/26/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "World.h"

World::World() {}

void World::PlaceBlocks() {
    
    Entity * projectile = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 0.0f, TILEHEIGHT * 6.0, -4.0f, 0.0f);
    projectiles.push_back(projectile);
    Entity * projectile2 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 0.0f, TILEHEIGHT * 7.0, -4.0f, 0.0f);
    projectiles.push_back(projectile2);
    
    for ( size_t j = 0; j < RAINDROPS; j++ ) {
        Entity * raindrop = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 0.0f, TILEHEIGHT * 4.0, 0.0, -3.33);
        rain.push_back(raindrop);
    }
    
    float randX, randY;
    
    for ( size_t i = 0; i < BLOCKS; i++ ) {
        randX = (rand() % 16 - 8) / 10.0;
        randY = (rand() % 1600) / 10.0;
        Entity * cloud = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 0.0, TILEHEIGHT * 2, randX, randY);
        blocks.push_back(cloud);
    }
    
    Entity * minus1 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 3.0f, TILEHEIGHT * 0.0, 0.05, 0.0);
    statics.push_back(minus1);
    Entity * plus1 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 3.0f, TILEHEIGHT * 1.0, 0.05, 0.0);
    statics.push_back(plus1);
    
    // bubbles
    Entity * outofbounds = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 3.0f, TILEHEIGHT * 2.0, 0.05, 0.0);
    statics.push_back(outofbounds);
    Entity * outofbounds2 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 3.0f, TILEHEIGHT * 2.0, 0.05, 0.0);
    statics.push_back(outofbounds2);
    
    // miniatures
    Entity * outofbounds1 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 1.0f, TILEHEIGHT * 1.0, 0.05, 0.0);
    statics.push_back(outofbounds1);
    Entity * outofbounds3 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 1.0f, TILEHEIGHT * 1.0, 0.05, 0.0);
    statics.push_back(outofbounds3);
    
    Entity * panelRight = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 10.0f, TILEHEIGHT * 0.0, 0.93, 0.0);
    panelRight->height = TILEHEIGHT * 7.0;
    statics.push_back(panelRight);
    
    Entity * panelLeft = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 11.0f, TILEHEIGHT * 0.0, -0.93, 0.0);
    panelLeft->height = TILEHEIGHT * 7.0;
    statics.push_back(panelLeft);
    
    // Menu items
    
    Entity * players = new Entity(LoadTexture(menuSheet), TILEWIDTH * 0.0f, TILEHEIGHT * 0.2, 0.0f, 0.0f);
    players->width = TILEWIDTH * 10.0;
    players->height = TILEHEIGHT * 8.0;
    menuItems.push_back(players);
    Entity * s = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 3.0f, TILEHEIGHT * 3.0, 0.0, 0.0);
    menuItems.push_back(s);
    Entity * s1 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 3.0f, TILEHEIGHT * 4.0, 0.0, 0.0);
    menuItems.push_back(s1);
    Entity * decor = new Entity(LoadTexture(menuSheet), TILEWIDTH * 10.5f, TILEHEIGHT * 0.0, 0.94, 0.0);
    decor->height = TILEHEIGHT * 10.0f;
    decor->width = TILEWIDTH * 3.0f;
    menuItems.push_back(decor);
    
    Entity * v = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 10.0f, TILEHEIGHT * 7.0, 0.0, 0.4);
    v->width = TILEWIDTH * 3.5f;
    menuItems.push_back(v);
    
    // Buttons
    for ( int k = 0; k < 6; k++ ) {
        Entity * b = new Entity(LoadTexture(spriteSheet), TILEWIDTH * (4.0f + k), TILEHEIGHT * 6.0, -0.95f, 0.0f);
        buttons.push_back(b);
        
    }
    
    for ( int l = 0; l < 6; l++ ) {
        Entity * b2 = new Entity(LoadTexture(spriteSheet), TILEWIDTH * (4.0f + l), TILEHEIGHT * 8.0, 0.95, 0.0f);
        buttons.push_back(b2);
        
    }

}

void World::AnchorStatics() {
    statics[6]->y = platform->y + 0.35;
    statics[7]->y = platform->y + 0.35;
    for ( size_t i = 0; i < buttons.size()/2; i++ ) {
        buttons[i]->y = platform->y + i/7.0;
    }
    for ( size_t i = buttons.size()/2; i < buttons.size(); i++ ) {
        buttons[i]->y = platform->y + (i - buttons.size()/2)/7.0;
    }
    if ( player2->hit ) {
        statics[0]->y = player2->y;
        statics[0]->x = player2->x;
        statics[1]->y = player->y;
        statics[1]->x = player->x;
    }
    if ( player->hit ) {
        statics[1]->y = player2->y;
        statics[1]->x = player2->x;
        statics[0]->y = player->y;
        statics[0]->x = player->x;
    }
    if ( player->y > platform->y + 1.2 ) {
        statics[2]->y = platform->y + 1.1;
        statics[2]->x = player->x;
        statics[4]->u = player->u;
        statics[4]->v = player->v;
        statics[4]->y = platform->y + 1.1;
        statics[4]->x = statics[2]->x;
    }
    else {
        statics[2]->x = 100;
        statics[2]->y = 100;
        statics[4]->x = 100;
        statics[4]->y = 100;
    }
    if ( player2->y > platform->y + 1.2 ) {
        statics[3]->y = platform->y + 1.1;
        statics[3]->x = player2->x;
        statics[5]->u = player2->u;
        statics[5]->v = player2->v;
        statics[5]->y = platform->y + 1.1;
        statics[5]->x = statics[3]->x;
    }
    else {
        statics[3]->x = 100;
        statics[3]->y = 100;
        statics[5]->x = 100;
        statics[5]->y = 100;
    }
}

void World::MovePlatform() {
    platform->acceleration_y = 0.0;
    platform->velocity_y = 1.0;
    platform->velocity_y += platform->acceleration_y * FIXED_TIMESTEP;
    platform->y += platform->velocity_y * FIXED_TIMESTEP;
}

void World::WeatherCheck() {
    
    raining = false;
    snowing = false;
    inSpace = false;
    
    if ( platform->y < SNOW_TRANSITION )
        raining = true;
    else if ( platform->y >= SNOW_TRANSITION && platform->y < SPACE_TRANSITION )
        snowing = true;
    else if ( platform->y >= SPACE_TRANSITION && platform->y < END_TRANSITION )
        inSpace = true;
    
    if ( fabs(player->y - SNOW_TRANSITION ) <= 1.0 ) EnterVortex();
}

void World::EnterVortex() {
    
    // effect that makes weather transition better
    
}

void World::Lightning() {
    if ( raining && ( SDL_GetTicks() % 123 == 0 || SDL_GetTicks() % 124 == 0 ) ) {
        glClearColor(0.4, 0.42, 0.46, 1.0);
    }
    else {
        if ( raining || snowing )
            glClearColor(fabs(0.1 + platform->y / 300.0),
                         fabs(0.11 + platform->y / 300.0),
                         fabs(0.13 + platform->y / 150.0), 1.0);
        if ( platform->y >= SPACE_TRANSITION - 5.0 )
            // + 4.9 is to start the sky darkening before entering space
            glClearColor((0.3 - (platform->y - SPACE_TRANSITION + 5.1) / 25.0),
                         (0.31 - (platform->y - SPACE_TRANSITION + 5.1) / 25.0),
                         (0.53 - (platform->y - SPACE_TRANSITION + 5.1) / 15.0), 1.0);
    }
}

void World::Rain() {
    
    for ( size_t i = 0; i < rain.size(); i++ ) {
        
        rain[i]->solid = false;
        
        rain[i]->velocity_x = 0.0;
        rain[i]->velocity_y = -1.5f;
        
        rain[i]->velocity_y += rain[i]->acceleration_y * FIXED_TIMESTEP;
        rain[i]->y += rain[i]->velocity_y * FIXED_TIMESTEP;
        rain[i]->velocity_x += rain[i]->acceleration_x * FIXED_TIMESTEP;
        rain[i]->x += rain[i]->velocity_x * FIXED_TIMESTEP;
        
        if ( rain[i]->y < platform->y - 2.0f ) {
            rain[i]->x = ((float)rand())/RAND_MAX * 2.66 - 1.33;
            rain[i]->y = platform->y + 3.0f;
            rain[i]->acceleration_y = (float)(rand() % 100 ) * -1;
        }
        if ( rain[i]->x > platform->x + 2.0f ) {
            rain[i]->x = ((float)rand())/RAND_MAX * 2.66 - 1.33;
            rain[i]->velocity_x = ((float)rand())/RAND_MAX * 0.5 - 0.25;
        }
    }
    
    for ( size_t i = 0; i < blocks.size(); i++ ) {
        blocks[i]->u = TILEWIDTH * 0.0;
    }
}

void World::Snow() {
    
    for ( int i = 0; i < rain.size(); i++ ) {
        
        rain[i]->solid = true;
        
        rain[i]->velocity_x = 0.01;
        rain[i]->velocity_y = -0.005f;
        
        rain[i]->velocity_y += rain[i]->acceleration_y * FIXED_TIMESTEP;
        rain[i]->y += rain[i]->velocity_y * FIXED_TIMESTEP;
        rain[i]->velocity_x += rain[i]->acceleration_x * FIXED_TIMESTEP;
        rain[i]->x += rain[i]->velocity_x * FIXED_TIMESTEP;
        
        if ( rain[i]->y < platform->y - 2.0f ) {
            rain[i]->x = ((float)rand())/RAND_MAX * 2.66 - 1.33;
            rain[i]->y = platform->y + 2.0f;
        }
        if ( rain[i]->x > platform->x + 2.0f ) {
            rain[i]->x = ((float)rand())/RAND_MAX * 2.66 - 1.33;
        }
    }
    
    for ( size_t i = 0; i < blocks.size(); i++ ) {
        blocks[i]->v = TILEHEIGHT * 2.0;
    }
    
    for ( size_t i = 0; i < rain.size(); i++ ) {
        rain[i]->v = TILEHEIGHT * 3.0;
    }
    
    platform->v = TILEHEIGHT * 2.0;
}

void World::Space() {
    
    
    for ( int i = 0; i < rain.size(); i++ ) {
        rain[i]->solid = true;
        
        rain[i]->velocity_x = 0.01;
        rain[i]->velocity_y = -0.005f;
        
        rain[i]->velocity_y += rain[i]->acceleration_y * FIXED_TIMESTEP;
        rain[i]->y += rain[i]->velocity_y * FIXED_TIMESTEP;
        rain[i]->velocity_x += rain[i]->acceleration_x * FIXED_TIMESTEP;
        rain[i]->x += rain[i]->velocity_x * FIXED_TIMESTEP;
    }
    
    for ( size_t i = 0; i < blocks.size(); i++ ) {
        blocks[i]->v = TILEHEIGHT * 5.0;
        if ( i % 10 == 0 ) blocks[i]->v = TILEHEIGHT * 1.0;
        else if ( i % 5 == 0 ) blocks[i]->v = TILEHEIGHT * 0.0;
        
        if ( blocks[i]->y < platform->y - 2.0f ) {
            blocks[i]->x = ((float)rand())/RAND_MAX * 2.66 - 1.33;
            blocks[i]->y = platform->y + 4.0 + ( rand() % 16 - 8)/ 10.0;
        }
    }
    
    platform->v = TILEHEIGHT * 4.0;
    platform->velocity_y = 0.4;
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