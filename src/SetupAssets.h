#include "stb_image.h"
// Ai code //
struct IconTexture {
    GLuint textureID;
    int width;
    int height;
};

IconTexture LoadTextureFromFile(const char* filename) {
    IconTexture result = {0, 0, 0};
    
    // Load image
    int channels;
    unsigned char* data = stbi_load(filename, &result.width, &result.height, &channels, 4);
    if (!data) {
        printf("Failed to load texture: %s\n", filename);
        return result;
    }
    
    // Create OpenGL texture
    glGenTextures(1, &result.textureID);
    glBindTexture(GL_TEXTURE_2D, result.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, result.width, result.height, 
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    return result;
}

// 2. Setup your assets (in initialization)
class AssetManager {
public:
    IconTexture playIcon;
    IconTexture pauseIcon;
    IconTexture stopIcon;
    IconTexture deleteIcon;
    IconTexture muteIcon;
    IconTexture soloIcon;
    IconTexture instrumentIcon;
    IconTexture divisionSnapIcon;
    IconTexture snipIcon;

    IconTexture boxSelect;
    
    void LoadAll() {
        playIcon = LoadTextureFromFile("../assets/icons/play.png");
        //pauseIcon = LoadTextureFromFile("../assets/icons/pause.png");
        //stopIcon = LoadTextureFromFile("../assets/icons/stop.png");
        boxSelect = loadTextureFromFile("../assets/icons/boxSelect.png");
    }
    
    void Cleanup() {
        glDeleteTextures(1, &playIcon.textureID);
        glDeleteTextures(1, &pauseIcon.textureID);
        glDeleteTextures(1, &stopIcon.textureID);
    }
};

// Global or member variable
AssetManager assets;
