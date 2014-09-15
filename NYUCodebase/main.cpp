
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

/* 
 
 Laura Barbera
 CS3113 - assignment1
 github.com/laupsy
 
    "Create a simple 2D scene using textured and untextured polygons.
     You can use any images you want, but feel free to use the assets in the class github repo.
     At least one element must be animated.
     You must use at least 3 different textures.
     At least one element of the scene must use vertex colors.
     Commit the source to your github repository and email me the link."
 
 */


SDL_Window* displayWindow;


// loads texture - this functiond does not work with commented out line..

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    
    return textureID;
}

// Draws a sprite from texture loaded from LoadTexture

void DrawSprite(GLint texture, float x, float y, float rotation, float scale) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale);
    GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}

int main(int argc, char *argv[]) {
    
    // Initial setup
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("laupsygame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    bool done = false;
    SDL_Event event;
    
    // Viewport stuff
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    // Textures
    
    GLuint happyBlob = LoadTexture("happyBlob.png");
    GLuint sadBlob = LoadTexture("sadBlob.png");
    
    // Animation stuff

    float prev = 0.0f; // init frame spot
    float fastRotate = 5.0f;
    float slowRotate = 0.0f;
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        
        // Draw start
        
        // Additional animation configuration
        
        float ticks = (float)SDL_GetTicks()/ 100.0f;
        float cur = ticks - prev;
        prev = ticks;
        
        // Clear screen
        glClearColor(0.3f, 0.4f, 0.2f, 1.0f); // Mountains
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // These blobs animate steadily - therefore, they are happy!
        
        slowRotate += (cur * 5.0f);
        DrawSprite(happyBlob, 0.15, -0.7, slowRotate, 0.3);
        DrawSprite(happyBlob, 0.10, -0.5, slowRotate, 0.3);
        DrawSprite(happyBlob, 0.8, -0.6, slowRotate, 0.3);
        
        // These blobs animate crazily - therefore, they are sad!
        
        fastRotate += (cur * 35.0f);
        DrawSprite(sadBlob, -0.6, -0.3, fastRotate, 0.3);
        DrawSprite(sadBlob, -0.8, -0.6, fastRotate, 0.3);
        DrawSprite(sadBlob, -0.15, -0.5, fastRotate, 0.3);
        
        // Sunrise thing
        glLoadIdentity();
        glScalef(0.1, 0.1, 0.1);
        glTranslatef(0.0, 6.5, 0.0);
        
        // An attempt to look like a sunrise over mountain peaks
        GLfloat sunriseLeft [] = { -8.0, 8.0, 6.0, -6.0, 4.0, 6.0, -3.0, -2.0, 5.0, -5.0 };
        //GLfloat sunriseRight [] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 5.0, -5.0 };
        
        glVertexPointer(2, GL_FLOAT, 0, sunriseLeft);
        //glVertexPointer(2, GL_FLOAT, 0, sunriseRight);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        
        // Colors
        
        GLfloat sunsetColors[] = { 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.5, 1.0, 0.5 };
        glColorPointer(3, GL_FLOAT, 0, sunsetColors);
        glEnableClientState(GL_COLOR_ARRAY);
        glDrawArrays(GL_POLYGON, 0, 10);
        glDisableClientState(GL_COLOR_ARRAY);
        
        // Draw end
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}