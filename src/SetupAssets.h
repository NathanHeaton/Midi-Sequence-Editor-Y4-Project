#pragma once
#include <cstdint>

using GLuint = uint32_t;

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
    IconTexture divisionSnapIcon;
    IconTexture snipIcon;
    IconTexture zoomIn;
    IconTexture zoomOut;
    IconTexture editIcon;
    IconTexture cutIcon;
    IconTexture moveIcon;
    IconTexture seekStart;
    IconTexture seekEnd;


    IconTexture boxSelect;
    void LoadAll();
    void Cleanup();

};

extern AssetManager ASSETS;
