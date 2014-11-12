
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
    player = new Entity(LoadTexture("laurasfirstsprite.png"), 36.0f/360.0f, 0.0f, 0.0f, 0.0f);
    // get map from txt file
    ///////
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
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    // window color
    glClearColor(0.55, 0.76, 1.0, 1.0);
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
    
    // keyboard stuff
    if ( keys[SDL_SCANCODE_LEFT] )
        player->velocity_x = -1.0f;
    else if ( keys[SDL_SCANCODE_RIGHT] )
        player->velocity_x = 1.0f;
    else
        player->velocity_x = 0.0f;
    // jumping
    if ( keys[SDL_SCANCODE_SPACE] ) {
        // check if colliding on bot
        /////
        // check if already jumping
        /////
        // jump
        player->velocity_y = 1.0f;
        // play jump sound
        /////

    }
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
    //// always apply particle trail when running
    //// player effect when selected (during champ select)
    //// squish and resize player when land from jump
    //// flatten player when ran over
    //// bullets have particle trail
    ///////
    // move the player with fixed timestep
    player->Go();
    ///////
    // check player collision
    ///////
    // run any AI eg enemies
    ///////
}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Render the level (things includes drawing stuff
    player->Draw(SCALE);
    ///////
    // Lock camera onto player
    ///////
    SDL_GL_SwapWindow(displayWindow);
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
