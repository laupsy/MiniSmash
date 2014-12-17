
#include "World.h"

using namespace std;

class Game {
public:
    
    Game();
    ~Game();
    
    void Init();
    void LoadObjects();
    void Loop();
    void Reset();
    
    void Update(float elapsed);
    void FixedUpdate();
    void Render();
    void StartMenu();
    void RenderStart();
    void RenderLevel();
    void RenderGameOver();
    
    bool UpdateAndRender();
    
    void PlayerControls(const Uint8 *keys, SDL_Event event);
    void PlayerBehavior(Entity * e);
    void Lightning();
    void CollisionCheck(Entity * e);
    void ProjectileCheck(Entity * e);
    void ShootProjectile(Entity * e, Entity * p);
    
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
    bool shake;
    bool startGame;
    bool endGame;
    
    float elapsed;
    float lastFrameTicks;
    float timeLeftOver;
    
    float camY = 0.0f;
    
    Mix_Music *music;
    Mix_Chunk *storm;
    Mix_Chunk *laser;
    Mix_Chunk *clank;
    Mix_Chunk *selection;
    Mix_Chunk *battleCryP1_1;
    Mix_Chunk *battleCryP1_2;
    Mix_Chunk *battleCryP2_1;
    Mix_Chunk *battleCryP2_2;
    Mix_Chunk *explosion;
    Mix_Chunk *energy;
    
    GLuint mainSpriteTexture;
    SDL_Event event;
    SDL_Window* displayWindow;
    
    
};
