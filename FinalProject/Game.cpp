
#include "Game.h"

Game::Game() {
    // init
    Init();
    // load objects
    LoadObjects();
    // check loop
    Loop();
}

void Game::LoadObjects() {
    
    // initialize the player
    player = new Entity(LoadTexture(spriteSheet), TILEWIDTH * playerFloating, TILEHEIGHT * cat, 0.0f, DEFAULT_Y);
    
    // create raindrops
    for ( int i = 0; i < 400; i++ ) {
        Entity * raindrop = new Entity(LoadTexture(spriteSheet), TILEWIDTH * 3.0, TILEHEIGHT * 2.0, -1.33f + i / 100.0, player->y + rand() % 10 - 5);
        rain.push_back(raindrop);
    }
    
    // start player as floating
    player->floating = true;
    
    //placeEntities(cloudForeground);
    
    placeEntities(blockBackground);
    placeEntities(blockForeground);
    
    //placeEntities(pinkPuff);
    //placeEntities(bluePuff);
    ///////
}

void Game::placeEntities(int whichEntity) {
    
    int amt;
    
    float randXLoc, randYLoc, offset;
    
    if ( whichEntity == blockForeground)
        amt = 18;
    else if ( whichEntity == blockBackground)
        amt = 18;
    else if ( whichEntity == pinkPuff )
        amt = 100;
    else if ( whichEntity == bluePuff )
        amt = 34;
    else if ( whichEntity == cloudForeground )
        amt = 100;
    else
        amt = 0;
    
    for ( size_t i = 0; i < amt; i++ ) {
        
        if ( whichEntity == blockForeground ) {
            
            // generate random positions
            randYLoc =  ( rand() % LEVELWIDTH * 10 - LEVELWIDTH/2 ) / 10.0f; // convert to float
            if ( ( rand() % 10 ) % 2 == 0 ) randXLoc = -1.66f;
            else randXLoc = -0.1f;
            
            // prevent overlap
            offset = (rand() % 10 ) / 10.0f - TILEWIDTH*2;
            if ( entities.size() > 0 ) {
                Entity * prevEntity = entities[entities.size() - 1];
                if ( prevEntity->y < randYLoc + offset && prevEntity->y > randYLoc - offset )
                    randYLoc += offset;
            }
            
            for ( int i = 0; i < 7; i++ ) {
                Entity * block = new Entity(LoadTexture(spriteSheet), TILEWIDTH * whichEntity, TILEHEIGHT * 2.0f, randXLoc + TILEWIDTH * i + 0.5, randYLoc);
                // make snowing block
                entities.push_back(block);
            }
            
            Entity * midblock = new Entity(LoadTexture(spriteSheet), TILEWIDTH * whichEntity, TILEHEIGHT * 2.0f, randXLoc + (rand() % 10 - 5) / 10.0, randYLoc - 0.5f);
            entities.push_back(midblock);
            
        }
        
        if ( whichEntity == blockBackground ) {
            
            // generate random positions
            randXLoc =  ( rand() % LEVELWIDTH * 10 - LEVELWIDTH/2*10 ) / 10.0f; // convert to float
            randYLoc = ( rand() % LEVELWIDTH * 10 - LEVELWIDTH/2*10 ) / 10.0f;
            
            // prevent overlap
            offset = (rand() % 10 ) / 10.0f + TILEWIDTH;
            if ( entities.size() > 0 ) {
                Entity * prevEntity = entities[entities.size() - 1];
                if ( prevEntity->x < randXLoc + offset && prevEntity->x > randXLoc - offset )
                    randXLoc += offset;
                if ( prevEntity->y < randYLoc + offset && prevEntity->y > randYLoc - offset ) {
                    if ( randYLoc + offset > 1.33  || randYLoc + offset < -1.33 ) randYLoc = 0.0f;
                    else randYLoc += offset;
                }
            }
            Entity * blockbg = new Entity(LoadTexture(spriteSheet), TILEWIDTH * whichEntity, TILEHEIGHT * 2.0f, randXLoc + TILEWIDTH, randYLoc);
            blockbg->solid = false;
            entities.push_back(blockbg);
        }
        
        if ( whichEntity == cloudForeground ) {
            
            // generate random positions
            randXLoc =  ( rand() % LEVELWIDTH * 10 - LEVELWIDTH/2*10 ) / 10.0f; // convert to float
            randYLoc = ( rand() % LEVELWIDTH * 10 - LEVELWIDTH/2*10 ) / 10.0f;
            
            // prevent overlap
            offset = (rand() % 10 ) / 10.0f + TILEWIDTH;
            if ( entities.size() > 0 ) {
                Entity * prevEntity = entities[entities.size() - 1];
                if ( prevEntity->x < randXLoc + offset && prevEntity->x > randXLoc - offset )
                    randXLoc += offset;
            }
            Entity * cloud = new Entity(LoadTexture(spriteSheet), TILEWIDTH * whichEntity, TILEHEIGHT * 2.0f, randXLoc + TILEWIDTH, randYLoc);
            cloud->solid = false;
            entities.push_back(cloud);
        }
    }
}

void Game::Loop() {
    // general vars
    done = false;
    lastFrameTicks = 0.0f;
    timeLeftOver = 0.0f;
    // any other game info that i might want loop eg score
    /////
}

Game::~Game() {
    // delete all objects
    delete player;
    for ( size_t i = 0; i < entities.size(); i++ ) {
        delete entities[i];
    }
    ///////
    // stop all music and sounds
    Mix_FreeMusic(music);
    // quit
    SDL_Quit();
}
void Game::Init() {
    // init chunk
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("laupsygame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800, 600,SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    // init audo
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    // window stuff
    glViewport(0,0,800,600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-2.33, 2.33, -1.0, 1.0, -1.0, 1.0);
    // window color
    glClearColor(0.2, 0.22, 0.26, 1.0);
    // reset frame
    glClear(GL_COLOR_BUFFER_BIT);
    // init music
    music = Mix_LoadMUS("battlestargalactica_roslinandadama.mp3");
    thunder = Mix_LoadWAV("thunder.wav");
    // play music
    //Mix_PlayMusic(music, -1);
}

void Game::Update(float elapsed) {
    
    // init keyboard
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    // lightning
    if ( raining && ( SDL_GetTicks() % 123 == 0 || SDL_GetTicks() % 124 == 0 ) ) {
        glClearColor(0.4, 0.42, 0.46, 1.0);
        Mix_PlayChannel(-1, thunder, 0);
    }
    else {
        glClearColor(fabs(0.2 + player->y / 25.0), fabs(0.22 + player->y / 25.0), fabs(0.26 + player->y / 10.0), 1.0);
    }
    
    // regular movement
    if ( keys[SDL_SCANCODE_LEFT] ) {
        if ( raining ) player->u = TILEWIDTH * playerWalkingLeft;
        if ( snowing ) player->u = TILEWIDTH * earmuffsWalkingLeft;
        if ( player->floating )
            player->velocity_x = VELOCITY_X * -1 / 10.0;
        else
            player->velocity_x = VELOCITY_X * -1;
    }
    else if ( keys[SDL_SCANCODE_RIGHT] ) {
        if ( raining ) player->u = TILEWIDTH * playerWalkingRight;
        if ( snowing ) player->u = TILEWIDTH * earmuffsWalkingRight;
        if ( player->floating )
            player->velocity_x = VELOCITY_X / 10.0;
        else
            player->velocity_x = VELOCITY_X;
    }
    else if ( keys[SDL_SCANCODE_DOWN] ) {
        if ( player->floating ) {
            player->velocity_y += VELOCITY_Y * -1 / 50;
            player->velocity_x = 0.0f;

        }
    }
    else if ( keys[SDL_SCANCODE_UP] ) {
        if ( player->floating ) {
            player->velocity_y += VELOCITY_Y / 50;
            player->velocity_x = 0.0f;
        }
    }
    else if ( keys[SDL_SCANCODE_F] ) {
        player->velocity_y = -0.5f;
        player->floating = true;
    }
    else
        player->velocity_x = 0.0f;
    
    // sprite
    if ( player->floating ) {
        if ( raining ) player->u = TILEWIDTH * playerFloating;
        if ( snowing ) player->u = TILEWIDTH * earmuffsFloating;
    }
    else {
        if ( player->velocity_x < 0 ) {
            if ( raining ) player->u = TILEWIDTH * playerWalkingLeft;
            if ( snowing ) player->u = TILEWIDTH * earmuffsWalkingLeft;
        }
        else if ( player->velocity_x > 0 ) {
            if ( raining ) player->u = TILEWIDTH * playerWalkingRight;
            if ( snowing ) player->u = TILEWIDTH * earmuffsWalkingRight;
        }
        else {
            if ( raining ) player->u = TILEWIDTH * playerStanding;
            if ( snowing ) player->u = TILEWIDTH * earmuffsFloating;
        }
    }
    
    if ( player->collidesBottom ) {
        player->velocity_y = 0.0f;
    }
    
    if ( player->collidesTop ) {
        player->velocity_y = VELOCITY_Y * -1;
    }
    
    if ( player->collidesLeft ) {
        player->velocity_x = 0;
    }
    
    if ( player->collidesRight ) {
        player->velocity_x = 0;
    }
    
    if ( player->x > 1.33 ) {
        player->x = -1.33;
    }
    
    if ( player->x < -1.33 ) {
        player->x = 1.33;
    }
    
    // sdl event thing
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            done = true;
        else if (event.type == SDL_KEYDOWN ) {
            if ( event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat ) {
                player->floating = false;
                // check if colliding on bot
                if ( player->collidesBottom ) {
                    player->collidesBottom = false;
                }
                /////
                // check if already jumping
                if ( !player->jumping ) {
                    player->velocity_y = 5.0f;
                }
                
                /////
                // jump
                ///// get a good speed to feel natural
                //player->velocity_y = 2.0f;
                // play jump sound
                /////￼
                // glLoadIdentity();
                // glPushMatrix();
                // glRotatef(15.0 * player->velocity_x,1.0,0.0,0.0);
                // glRotatef(15.0 * player->velocity_x,0.0,1.0,0.0);
                // glRotatef(15.0 * player->velocity_x,0.0,0.0,1.0);
                // glPopMatrix();
                // glScalef(1.0, -1.0, 1.0); this would be cool for like a reverse gravity mode
            }
        }
    }
}

void Game::FixedUpdate() {
    // play game sounds
    //// sound on shoot
    //// sound on death
    //// sound on speed boost
    //// sound on start
    //// sound on win
    //// sound on jump up
    //// sound on land from jump
    //// sound on crash into object
    //// sound on crash into player2
    //// sound on player select
    //// sound on lap
    ///////
    // do game effects
    //// player jump makes player scale up slightly and applies circle shadow below while moving up very slightly
    //// after a certain speed stretch player
    //// squish player if crash
    //// confetti on success (anything classified as success event will receive a bool)
    //////// make particle gen function and for this randomize colors from an array of colors (roygbv) and scale particles?
    //// always apply particle trail when running
    //// player effect when selected (during champ select)
    //// squish and resize player when land from jump
    //// flatten player when ran over
    //// bullets have particle trail
    ///////
    // move the player with fixed timestep
    if ( !player->floating ) player->Go();
    else player->Float();
    // weather effect
    Rain();
    ///////
    // check player location
    if ( player->y < -1.33 ) {
        player->floating = true;
        player->velocity_y = -0.15f;
        player->y = DEFAULT_Y;
    }
    // check player collision
    /////// reset scale/rotation on collision with bot
    ///////
    /////// check if colliding with speed boost
    /////// check if colliding with object that causes death
    /////// check if colliding with other player
    /////// check if colliding with platform
    player->collidesTop = false;
    player->collidesBottom = false;
    player->collidesLeft = false;
    player->collidesRight = false;
    for ( size_t i = 0; i < entities.size(); i++ ) { player->collidesWith(entities[i]); }
    // check weather
    raining = false;
    snowing = false;
    inSpace = false;
    if ( player->y < 3.0 ) raining = true;
    else if ( player->y >= 3.0 ) snowing = true;
    else inSpace = false;
    /////// check if colliding with bullet
    /////// check if colliding with finish line
    ////////// check order of collision with finish line (eg if player1 collides w finish line before player2, player1 wins)
    // run any AI eg enemies
    ///////
}

void Game::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if ( player->y > DEFAULT_Y )
        camY = player->y * -1;
    
    glTranslatef(0.0f, camY, 0.0f);
    
    // Render the level (things includes drawing stuff
    for ( size_t k = 0; k < clouds.size(); k++ ) {
        clouds[k]->Draw(SCALE);
    }
    for ( size_t j = 0; j < bg.size(); j++ ) {
        bg[j]->Draw(SCALE);
    }
    for ( size_t i = 0; i < entities.size(); i++ ) {
        entities[i]->Draw(SCALE);
    }
    
    // draw player last so not overlapped by solids
    player->Draw(SCALE);
    
    for ( size_t h = 0; h < rain.size(); h++ ) {
        rain[h]->Draw(SCALE * ( rand() % 10 / 10.0 ));
    }
    
    ///////
    // Lock camera onto player
    ////// will need to change camera position so that both players are always visible
    ///////// do this after all single-player game stuff is set, then add second player and go from there?
    ///////
    glPopMatrix();
    SDL_GL_SwapWindow(displayWindow);
}

void Game::Rain() {
    for ( size_t i = 0; i < rain.size(); i++ ) {
        
        if ( raining ) {
            rain[i]->u = TILEWIDTH * 3.0;
            rain[i]->velocity_y = (rand() % 6 * -1);
            rain[i]->velocity_x = 0.0f;
            rain[i]->floating = false;
            rain[i]->Go();
        }
        else if ( snowing ) {
            rain[i]->u = TILEWIDTH * 4.0;
            rain[i]->Flutter();
            if ( rain[i]->x < player->x - 0.66 ) rain[i]->FlutterLeft();
        }
        
        if ( rain[i]->y < player->y - 1.33 ) rain[i]->y = player->y + 1.33;
    }
}

bool Game::UpdateAndRender() {
    // time elapsed stuff
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    // fixed time elapsed stuff
    float fixedElapsed = elapsed + timeLeftOver;
    if ( fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS )
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    // update timestep
    while ( fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        FixedUpdate();
    }
    
    timeLeftOver = fixedElapsed;
    
    // update and render
    Update(elapsed);
    Render();
    
    // quit if applicable
    return done;
}
GLuint Game::LoadTexture(const char *image_path) {
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
void Game::DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a ) {
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    glTranslatef( x, y, 0.0 );
    vector<float> vertexData;
    vector<float> textCoordData;
    vector<float> colorData;
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