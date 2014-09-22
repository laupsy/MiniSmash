
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include "Entity.h"


/* 
 
 Laura Barbera
 CS3113 - assignment2
 github.com/laupsy
 
 "
 - Make PONG!
 - Doesn’t need to keep score.
 - But it must detect player wins.
 - Can use images or basic shapes.
 - Can use keyboard, mouse or joystick.
 ￼￼￼￼￼"
 
 Player wins when 4 slices of cake are eaten!
 
 */


SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
    
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels); // textures were tinted blue.. so i swapped GL_RGBA for GL_BGRA
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void Setup() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("LaupsyPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_MODELVIEW);
}

bool checkHitVert ( Entity toCheck ) {
    return toCheck.getY() > 1.0 || toCheck.getY() > -1.0;
}

bool checkHitHoriz ( Entity toCheck, Entity pLeft, Entity pRight ) {
    
    return  (toCheck.getX() - toCheck.getWidth() / 2 < pRight.getX() + pRight.getWidth() / 2 &&
            toCheck.getY() > pRight.getY() - pRight.getWidth() / 2 &&
            toCheck.getY() < pRight.getY() + pRight.getWidth() / 2) ||
            (toCheck.getX() - toCheck.getWidth() / 2 > pLeft.getX() - pLeft.getWidth() / 2 - toCheck.getWidth() &&
            toCheck.getY() > pLeft.getY() - pLeft.getWidth() / 2 &&
            toCheck.getY() < pLeft.getY() + pLeft.getWidth() / 2);
}

void Render() {
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glClearColor(1.0, 0.8, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DrawGame(Entity leftPaddle, Entity rightPaddle, Entity ball) {
    leftPaddle.Draw();
    rightPaddle.Draw();
    ball.Draw();
}

int main(int argc, char *argv[]) {
    
    Setup();
    
    float lastFrameTicks = 0.0;
    float angle = 0.0;
    
    int slices = 8;
    
    // Winner is whoever eats the most slices before the cake is gone!
    
    Entity paddleOne(0.8, 0.0, 0.45, 0.12, LoadTexture("forkblue.png"));
    Entity paddleTwo(-0.8, 0.0, 0.45, 0.12, LoadTexture("forkpink.png"));
    Entity ball(0.0, 0.0, 0.2, 0.2, LoadTexture("cake1.png"));
    
    ball.setDirection( -0.2, -0.1 );
    ball.setSpeed( 2.0 );
    
    Render();
    
    int scoreBlue = 0;
    int scorePink = 0;
    
    bool done = false;
    SDL_Event event;
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        float ticks = (float)SDL_GetTicks() / 1000.0f; // extra 0 for smoother animation?  very shaky at 1000.0f
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        ball.setX( ( ball.getX() + (elapsed * ball.getDirectionX() * ball.getSpeed() ) )  );
        ball.setY( ( ball.getY() + (elapsed * ball.getDirectionY() * ball.getSpeed() ) )  );
        
        // START KEYBOARD STUFF
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        
        // Right side player
        
        if ( keys[SDL_SCANCODE_UP] ) {
            if ( paddleOne.getY() + elapsed < 1.0 ) {
                paddleOne.setY( paddleOne.getY() + elapsed ) ;
            }
        }
        
        else if ( keys[SDL_SCANCODE_DOWN] ) {
            if ( paddleOne.getY() - 0.2 + elapsed > -1.0 ) {
                paddleOne.setY( paddleOne.getY() - elapsed ) ;
            }
        }
        
        // Left side player
        
        else if ( keys[SDL_SCANCODE_W] ) {
            if ( paddleTwo.getY() + elapsed < 1.0 ) {
                paddleTwo.setY( paddleTwo.getY() + elapsed ) ;
            }
        }
        
        else if ( keys[SDL_SCANCODE_A] ) {
            if ( paddleTwo.getY() - 0.2 + elapsed > -1.0 ) {
                paddleTwo.setY( paddleTwo.getY() - elapsed ) ;
            }
        }
        
        // END KEYBOARD STUFF
        
        // Hits ceiling or floor
        
        if ( checkHitVert(ball) == true ) {
            
            // If the cake hits ceiling or floor, it must be replaced with a new one.
            // below doesnt work....
//            std::string newTexture = "cake" + std::to_string(rand()%5) + ".png";
//            ball.setTexture(LoadTexture(newTexture));
            
//            int randomCake = rand() % 5;
//            if ( randomCake == 1 ) ball.setTexture(LoadTexture("cake1.png"));
//            else if ( randomCake == 2 ) ball.setTexture(LoadTexture("cake2.png"));
//            else if ( randomCake == 3 ) ball.setTexture(LoadTexture("cake3.png"));
//            else if ( randomCake == 4 ) ball.setTexture(LoadTexture("cake4.png"));
//            else if ( randomCake == 5 ) ball.setTexture(LoadTexture("cake5.png"));
            
            ball.setDirection(ball.getDirectionX(), -1.0 * ball.getDirectionY());
        }

        // Hits left or right paddle
        
        if ( checkHitHoriz( ball, paddleOne, paddleTwo ) == true ) {
            slices--;
            ball.setDirection( -1.0 * ball.getDirectionX(), ball.getDirectionY() );
            // if direction is positive, it just came from left
            // keeps track of score
            if ( ball.getDirectionX() > 0 ) {
                std::cout << "Going right - One point for Blue side" << "\n" << "Pink: " << scorePink << " || " << "Blue: " << ++scoreBlue << "\n\n";
            }
            else {
                std::cout << "Going left - One point for Pink side" << "\n" << "Pink: " << ++scorePink << " || " << "Blue: " << scoreBlue << "\n\n";
            }
        }

        // Miss
        
        else if ( ball.getX() < -1.33 || ball.getX() > 1.33 ) ball.resetLoc();
        
        glClearColor(1.0, 0.9, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        
        DrawGame(paddleOne, paddleTwo, ball);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    
    // end game after 20 turns
    
    if ( scorePink > scoreBlue ) std::cout << "Pink side got the most cake!";
    else if ( scorePink < scoreBlue ) std::cout << "Blue side got the most cake!";
    else std::cout << "Both sides ate the same amount of cake.";
    
    SDL_Quit();
    return 0;
}