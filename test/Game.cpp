
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
    player = new Entity(LoadTexture("laurassecondsprite.png"), TILEWIDTH * 0.0, TILEHEIGHT * 0.0, -1.0f, -0.2f);
    // start player as floating
    player->floating = true;
    // get map from txt file
    //readLevel();
    //buildTiles();
//    Entity * rainbowStart = new Entity(LoadTexture("laurasfirstsprite.png"), SIZE * 0.0f, SIZE * 1.2, -0.58f, 0.0f);
//    rainbowStart->height = SIZE * 8.75;
//    rainbowStart->width = SIZE * 5.8;
//    entities.push_back(rainbowStart);
    
//    for ( size_t i = 0; i < 50; i++ ) {
//        Entity * raindrop = new Entity(LoadTexture("laurasfirstsprite.png"), SIZE * 1.0f, SIZE * 1, -0.58f, 0.0f);
//        rain.push_back(raindrop);
//    }
    
    placeEntities(blockBackground);
    placeEntities(cloudBackground);
    
    //placeEntities(cloudForeground);
    placeEntities(blockForeground);
    
    placeEntities(pinkPuff);
    placeEntities(bluePuff);
    ///////
}

void Game::placeEntities(int whichEntity) {
    
    int amt;
    
    float randXLoc, randYLoc, offset;
    
    if ( whichEntity == blockForeground)
        amt = 75;
    else if ( whichEntity == blockBackground)
        amt = 10;
    else if ( whichEntity == pinkPuff )
        amt = 100;
    else if ( whichEntity == bluePuff )
        amt = 34;
    else if ( whichEntity == cloudForeground )
        amt = 25;
    else if ( whichEntity == cloudBackground )
        amt = 15;
    else
        amt = 0;
    
    for ( size_t i = 0; i < amt; i++ ) {
        
        if ( whichEntity == blockForeground ) {
            
            // generate random positions
            randXLoc = (rand() % 500)/100.0;
            randYLoc = (rand() % 200 - 100)/100.0;
            
            offset = 0.15;
            
            // dont let blocks overlap
            for ( size_t i = 0; i < entities.size(); i++ ) {
                while ( entities[i]->x <= randXLoc + offset && entities[i]->x > randXLoc - offset ) {
                    randXLoc += offset;
                    randYLoc += offset;
                }
            }
            
            Entity * block = new Entity(LoadTexture("laurasfirstsprite.png"), TILEWIDTH * whichEntity, 0.0f, randXLoc, randYLoc);
            entities.push_back(block);
        }
        
        else if ( whichEntity == blockBackground ) {
            
            // generate random positions
            randXLoc = (rand() % 500 - 250)/100.0;
            randYLoc = (rand() % 200 - 100)/100.0;
            
            offset = 0.05;
            
            // dont let blocks overlap
            for ( size_t i = 0; i < bg.size(); i++ ) {
                while ( bg[i]->x <= randXLoc + offset && bg[i]->x > randXLoc - offset ) {
                    randXLoc += offset;
                    randYLoc += offset;
                }
                //                while ( entities[i]->y <= randYLoc + offset && entities[i]->y > randYLoc - offset ) {
                //                    randXLoc += offset;
                //                    randYLoc += offset;
                //                }
            }
            
            Entity * blockbg = new Entity(LoadTexture("laurasfirstsprite.png"), TILEWIDTH * whichEntity, 0.0f, randXLoc, randYLoc);
            bg.push_back(blockbg);
        }
        
        else if ( whichEntity == cloudForeground ) {
            
            // generate random positions
            randXLoc = (rand() % 500 - 250)/100.0;
            randYLoc = (rand() % 100 - 50)/100.0;
            
            offset = 0.2;
            
            // dont let blocks overlap
            for ( size_t i = 0; i < clouds.size(); i++ ) {
                while ( clouds[i]->x <= randXLoc + offset && clouds[i]->x > randXLoc - offset ) {
                    randXLoc += offset;
                    randYLoc += offset;
                }
                //                while ( entities[i]->y <= randYLoc + offset && entities[i]->y > randYLoc - offset ) {
                //                    randXLoc += offset;
                //                    randYLoc += offset;
                //                }
            }
            
            Entity * cloud = new Entity(LoadTexture("laurasfirstsprite.png"), TILEWIDTH * whichEntity, 0.0f, randXLoc, randYLoc);
            clouds.push_back(cloud);
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
    music = Mix_LoadMUS("music.mp3");
    // play music
    Mix_PlayMusic(music, -1);
}

void Game::Update(float elapsed) {
    
    // init keyboard
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    // regular movement
    if ( keys[SDL_SCANCODE_LEFT] ) {
        player->u = TILEWIDTH * playerWalkingLeft;
        if ( player->floating )
            player->velocity_x = VELOCITY_X * -1 / 10;
        else
            player->velocity_x = VELOCITY_X * -1;
    }
    else if ( keys[SDL_SCANCODE_RIGHT] ) {
        player->u = TILEWIDTH * playerWalkingRight;
        if ( player->floating )
            player->velocity_x = VELOCITY_X / 10;
        else
            player->velocity_x = VELOCITY_X;
    }
    else if ( keys[SDL_SCANCODE_DOWN] ) {
        if ( player->floating ) {
            player->velocity_y += VELOCITY_Y * -1 / 100;
            player->velocity_x = VELOCITY_X/10;
        }
    }
    else if ( keys[SDL_SCANCODE_UP] ) {
        if ( player->floating ) {
            player->velocity_y += VELOCITY_Y / 100;
            player->velocity_x = VELOCITY_X/10;
        }
    }
    else if ( keys[SDL_SCANCODE_F] ) {
        player->velocity_y = -0.5f;
        player->floating = true;
    }
    else
        player->velocity_x = 0.0f;
    
    // sprite
    if ( player->floating )
        player->u = TILEWIDTH * playerFloating;
    else {
        if ( player->velocity_x < 0 )
            player->u = TILEWIDTH * playerWalkingLeft;
        else if ( player->velocity_x > 0 )
            player->u = TILEWIDTH * playerWalkingRight;
        else
            player->u = TILEWIDTH * playerStanding;
    }
    
    // jumping
    if ( keys[SDL_SCANCODE_SPACE] ) {
        
        player->floating = false;
        // check if colliding on bot
        /////
        // check if already jumping
        /////
        // jump
        ///// get a good speed to feel natural
        player->velocity_y = 2.0f;
        // play jump sound
        /////￼
        glLoadIdentity();
        glPushMatrix();
        glRotatef(15.0 * player->velocity_x,1.0,0.0,0.0);
        glRotatef(15.0 * player->velocity_x,0.0,1.0,0.0);
        glRotatef(15.0 * player->velocity_x,0.0,0.0,1.0);
        glPopMatrix();
        // glScalef(1.0, -1.0, 1.0); this would be cool for like a reverse gravity mode
    }
    
    // collision response
    if ( player->IsColliding(entities) )
        player->floating = false;
    
    // sdl event thing
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            done = true;
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
    ///////
    // check player collision
    /////// reset scale/rotation on collision with bot
    ///////
    /////// check if colliding with speed boost
    /////// check if colliding with object that causes death
    /////// check if colliding with other player
    /////// check if colliding with platform
    /////// check if colliding with bullet
    /////// check if colliding with finish line
    ////////// check order of collision with finish line (eg if player1 collides w finish line before player2, player1 wins)
    // run any AI eg enemies
    ///////
}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
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
    for ( size_t h = 0; h < rain.size(); h++ ) {
        rain[h]->Draw(SCALE);
    }
    
    // draw player last so not overlapped by solids
    player->Draw(SCALE);

    ///////
    // Lock camera onto player
    ////// will need to change camera position so that both players are always visible
    ///////// do this after all single-player game stuff is set, then add second player and go from there?
    ///////
    SDL_GL_SwapWindow(displayWindow);
}

void Game::Rain() {
    
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

//void Game::buildTiles() {
//    // insert all leveldata information into vertextData and texCoordData
//    ///////
//    // translate call here will be commented out when centering camera to player
//    ///////
//    glLoadIdentity();
//    glTranslatef(-TILE_SIZE * mapWidth / 2, TILE_SIZE * mapHeight / 2, 0.0f);
//    ///////
//    glBindTexture(GL_TEXTURE_2D, LoadTexture("laurasfirstsprite.png"));
//    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    for ( int y = 0; y < LEVEL_HEIGHT; y++ ) {
//        for ( int x = 0; x < LEVEL_WIDTH; x++ ) {
//            
//            float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
//            float v = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
//            
//            float spriteWidth = 1.0 / (float) SPRITE_COUNT_X;
//            float spriteHeight = 1.0 / (float) SPRITE_COUNT_Y;
//            
//            vertexData.insert(vertexData.end(), {
//                TILE_SIZE * x, -TILE_SIZE * y,
//                TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
//                (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
//                (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
//            });
//            
//            texCoordData.insert(texCoordData.end(), { u, v,
//                u, v + (spriteHeight),
//                u + spriteWidth, v + (spriteHeight),
//                u + spriteWidth, v
//            });
//        }
//    }
//    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    glDrawArrays(GL_QUADS, 0, vertexData.size()/2);
//}
//
//void Game::buildLevel() {
//    //memcpy(levelData, level1Data, LEVEL_HEIGHT*LEVEL_WIDTH);
//}
//
//void Game::readLevel() {
//    string mapWidthTemp, mapHeightTemp, tileWidthTemp, tileHeightTemp;
//    // open file
//    ifstream mapFile;
//    mapFile.open("map.txt");
//    string line;
//    // loop through file
//    // separate each aspect of the file
//    if ( mapFile.is_open() ) {
//        while ( !mapFile.eof() ) {
//            getline(mapFile, line);
//            if ( line == "[header]") {
//                // traverse through all items under header until linebreak
//                ///////
//                while ( getline(mapFile, line) ) {
//                    if ( line == "" ) break;
//                    else {
//                        // separate strings by equal sign
//                        //////
//                        istringstream sStream(line);
//                        string key, value;
//                        getline(sStream, key, '=');
//                        getline(sStream, value);
//                        if ( key == "width" )
//                            mapWidth = atoi(value.c_str());
//                        else if ( key == "height" )
//                            mapHeight = atoi(value.c_str());
//                        else if ( key == "tilewidth" )
//                            tileWidth = atoi(value.c_str());
//                        else if ( key == "tileheight" )
//                            tileHeight = atoi(value.c_str());
//                    }
//                }
//            }
//            //else if ( line == "[tilesets]" ) {}
//            else if ( line == "[layer]" ) {
//                while ( getline(mapFile, line) ) {
//                    if ( line == "" ) break;
//                    else {
//                        istringstream sStream(line);
//                        string key, value;
//                        getline(sStream, key, '=');
//                        getline(sStream, value);
//                        if ( key == "data") {
//                            // remember to do it in reverse
//                            ///////
//                            for ( int y = 0; y < mapHeight; y++ ) {
//                                getline(mapFile, line);
//                                istringstream lineStream(line);
//                                string tile;
//                                for ( int x = 0; x < mapWidth; x++ ) {
//                                    getline(lineStream, tile, ',');
//                                    int val = (unsigned char)atoi(tile.c_str());
//                                    if ( val > 0 ){
//                                        levelData[y][x] = val - 1;
//                                        cout << levelData[y][x] << endl;
//                                    }
//                                    else {
//                                        levelData[y][x] = 0;
//                                        //cout << levelData[y][x] << endl;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    
//    //    cout << mapWidth << endl;
//    //    cout << mapHeight << endl;
//    //    cout << tileWidth << endl;
//    //    cout << tileHeight << endl;
//}
