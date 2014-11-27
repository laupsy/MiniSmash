#ifndef __NYUCodebase__World__
#define __NYUCodebase__World__


#include "Entity.h"

class World {
public:
    World();
    GLuint LoadTexture(const char *image_path);
    void DrawText(GLuint textTexture, std::string text, float x, float y, float spacing, float size, float r, float g, float b, float a);
    
    void PlaceBlocks();
    
    void Build();
    void Lightning();
    void Rain();
    void Snow();
    
    std::vector<Entity*> blocks;
    const char * spriteSheet = "laurassecondsprite.png";
};

#endif /* defined(__NYUCodebase__World__) */
