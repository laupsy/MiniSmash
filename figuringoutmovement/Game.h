
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
    
    float blockForeground = 0.0f;
    float blockBackground = 1.0f;
    float cloudForeground = 2.0f;
    
    float pinkPuff = 7.0f;
    float bluePuff = 7.0f;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    
    //    unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
    
    Mix_Music *music;
    
    GLuint mainSpriteTexture;
    SDL_Event event;
    SDL_Window* displayWindow;
    
    
};
