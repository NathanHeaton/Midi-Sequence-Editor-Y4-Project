//
// Created by nathan on 17/01/2026.
//
#include "SetupAssets.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <vector>

AssetManager ASSETS;

IconTexture LoadTextureFromFile(const char* filename) {
    IconTexture result = {0, 0, 0};

    // Load image
    int channels;
    unsigned char* data = stbi_load(filename, &result.width, &result.height, &channels, 4);
    if (!data) {
        printf("Failed to load texture: %s\n", filename);
        return result;
    }
    printf("texture width: %d\n ", result.width);

    // Create OpenGL texture
    glGenTextures(1, &result.textureID);
    glBindTexture(GL_TEXTURE_2D, result.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, result.width, result.height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    printf("texture id: %d\n ", result.textureID);
    return result;
}

void AssetManager::LoadAll() {
    playIcon = LoadTextureFromFile("assets/icons/play.png");
    //pauseIcon = LoadTextureFromFile("assets/icons/pause.svg");
    //stopIcon = LoadTextureFromFile("../assets/icons/stop.svg");
    //boxSelect = loadTextureFromFile("../assets/icons/box_select.svg");
}

void AssetManager::Cleanup() {
    glDeleteTextures(1, &playIcon.textureID);
    glDeleteTextures(1, &pauseIcon.textureID);
    glDeleteTextures(1, &stopIcon.textureID);
}
