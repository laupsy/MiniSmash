
#include "Entity.h"

using namespace std;

class Game {
public:
    
    Game();
    ~Game();
    
    void Init();
    void Update(float elapsed);
    void FixedUpdate();
    void Render();
    void RenderStart();
    void RenderLevel();
    void RenderGameOver();
    bool UpdateAndRender();
    
    void InitMusic();
    
    void EventsFromCollision(Entity * e);
    void SetCollisionToFalse(Entity * e);
    void Fall(Entity * e);
    void CheckCollisionX(Entity * e);
    void CheckCollisionY(Entity * e);
    
    GLuint LoadTexture(const char *image_path);
    void DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a );
    
    vector<Entity*> entities;
    
    Entity * player;
    
    bool done;
    
    int mapWidth;
    int mapHeight;
    
    float elapsed;
    float lastFrameTicks;
    float timeLeftOver;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    
    Mix_Music *music;
    
    const Uint8 *keys;
    
    GLuint mainSpriteTexture;
    SDL_Event event;
    SDL_Window* displayWindow;
    
    
};
