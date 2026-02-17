#pragma once

#include <filesystem>
#include <GL/gl.h>

// Ai code //
struct IconTexture {
    GLuint textureID;
    int width;
    int height;
};

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
    IconTexture saveIcon;
    IconTexture zoomIn;
    IconTexture zoomOut;
    IconTexture editIcon;
    IconTexture cutIcon;
    IconTexture moveIcon;


    IconTexture boxSelect;
    void LoadAll();
    void Cleanup();

};

extern AssetManager ASSETS;
