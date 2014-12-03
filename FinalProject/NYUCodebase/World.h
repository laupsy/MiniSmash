#ifndef __NYUCodebase__World__
#define __NYUCodebase__World__

#include "Entity.h"

class World {
public:
    
    World();
    
    void PlaceBlocks();
    void WeatherCheck();
    void Build();
    void Lightning();
    void Rain();
    void Snow();
    void Space();
    void EnterVortex();
    void MovePlatform();
    
    GLuint LoadTexture(const char *image_path);
    void DrawText(GLuint textTexture, std::string text, float x, float y, float spacing, float size, float r, float g, float b, float a);
    
    std::vector<Entity*> blocks;
    std::vector<Entity*> rain;
    std::vector<Entity*> projectiles;
    
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    
    const char * spriteSheet = "laurassecondsprite.png";
    
    bool raining, snowing, inSpace;
    
    Entity * player;
    Entity * platform;
    
    // Y AXIS
    float object = 0.0f;
    float cat = 1.0f;
    float rabbit = 2.0f;
    
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
    

};

#endif /* defined(__NYUCodebase__World__) */
