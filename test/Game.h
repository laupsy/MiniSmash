
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
    
    bool readHeader(std::ifstream &stream);
    bool readLayerData(std::ifstream &stream);
    bool readEntityData(std::ifstream &stream);
    
    void placeEntity(float placeX, float placeY);
    
    GLuint LoadTexture(const char *image_path);
    void DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a );
    
    vector<Entity*> entities;
    
    bool done;
    
    int mapWidth;
    int mapHeight;
    
    float elapsed;
    float lastFrameTicks;
    float timeLeftOver;
    
    const Uint8 *keys;
    
    GLuint mainSpriteTexture;
    SDL_Event event;
    SDL_Window* displayWindow;
    
};
