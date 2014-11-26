
#include "Entity.h"

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
    
    // void buildLevel();
    // void buildTiles();
    // void readLevel();
    void placeEntities(int whichEntity);
    void PlayerControls(Entity * e, const Uint8 *keys, SDL_Event event);
    void PlayerBehavior(Entity * e);
    void Lightning();
    void CollisionCheck();
    void WeatherCheck();
    
    void Rain();
    
    void InitMusic();
    
    void EventsFromCollision(Entity * e);
    void SetCollisionToFalse(Entity * e);
    void Fall(Entity * e);
    
    GLuint LoadTexture(const char *image_path);
    void DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a );
    
    vector<Entity*> entities;
    vector<Entity*> bg;
    vector<Entity*> clouds;
    vector<Entity*> rain;
    
    Entity * player;
    
    const char * spriteSheet = "laurassecondsprite.png";
    
    bool done;
    
    bool raining, snowing, inSpace;
    
    //    int mapWidth;
    //    int mapHeight;
    //    int tileWidth;
    //    int tileHeight;
    
    float elapsed;
    float lastFrameTicks;
    float timeLeftOver;
    
    // Y AXIS
    float cat = 0.0f;
    float rabbit = 1.0f;
    
    // X AXIS
    float playerFloating = 0.0f;
    float playerWalkingRight = 1.0f;
    float playerWalkingLeft = 2.0f;
    float playerStanding = 3.0f;
    float earmuffsFloating = 4.0f;
    float earmuffsWalkingLeft = 5.0f;
    float earmuffsWalkingRight = 6.0f;
    
    float blockForeground = 0.0f;
    float blockBackground = 1.0f;
    float cloudForeground = 2.0f;
    
    float pinkPuff = 7.0f;
    float bluePuff = 7.0f;
    
    float camY = 0.0f;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    
    //    unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
    
    Mix_Music *music;
    Mix_Music *storm;
    Mix_Chunk *thunder;
    
    GLuint mainSpriteTexture;
    SDL_Event event;
    SDL_Window* displayWindow;
    
    
};
