
#include "World.h"

using namespace std;

class Game {
public:
    
    Game();
    ~Game();
    
    void Init();
    void LoadObjects();
    void Loop();
    
    void Update(float elapsed);
    void FixedUpdate();
    void Render();
    void RenderStart();
    void RenderLevel();
    void RenderGameOver();
    bool UpdateAndRender();
    
    void PlayerControls(Entity * e, const Uint8 *keys, SDL_Event event);
    void PlayerBehavior(Entity * e);
    void Lightning();
    void CollisionCheck();
    void ProjectileCheck();
    
    void Rain();
    
    void InitMusic();
    
    void EventsFromCollision(Entity * e);
    void SetCollisionToFalse(Entity * e);
    void Fall(Entity * e);
    
    vector<Entity*> bg;
    vector<Entity*> clouds;
    vector<Entity*> rain;

    World * world;
    
    bool done;
    
    float elapsed;
    float lastFrameTicks;
    float timeLeftOver;
    
    float camY = 0.0f;
    
    Mix_Music *music;
    Mix_Music *storm;
    Mix_Chunk *thunder;
    
    GLuint mainSpriteTexture;
    SDL_Event event;
    SDL_Window* displayWindow;
    
    
};
