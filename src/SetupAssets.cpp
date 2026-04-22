//
// Created by nathan on 17/01/2026.
//
#include "SetupAssets.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//
#include <juce_opengl/juce_opengl.h>

using namespace juce::gl;
AssetManager ASSETS;

IconTexture loadTextureFromFile(const char* filename) {
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
    playIcon = loadTextureFromFile("assets/icons/play.png");
    pauseIcon = loadTextureFromFile("assets/icons/pause.png");
    //stopIcon = loadTextureFromFile("../assets/icons/stop.png");
    boxSelect = loadTextureFromFile("assets/icons/box_select.png");
    editIcon = loadTextureFromFile("assets/icons/edit.png");
    cutIcon = loadTextureFromFile("assets/icons/cut.png");
    divisionSnapIcon = loadTextureFromFile("assets/icons/snapping.png");
    zoomIn = loadTextureFromFile("assets/icons/zoom_in.png");
    zoomOut = loadTextureFromFile("assets/icons/zoom_out.png");
    moveIcon = loadTextureFromFile("assets/icons/move.png");
    deleteIcon = loadTextureFromFile("assets/icons/delete.png");
    seekStart = loadTextureFromFile("assets/icons/seek_start.png");
    seekEnd = loadTextureFromFile("assets/icons/seek_end.png");
}

void AssetManager::Cleanup() {
    glDeleteTextures(1, &playIcon.textureID);
    glDeleteTextures(1, &pauseIcon.textureID);
    //glDeleteTextures(1, &stopIcon.textureID);
    glDeleteTextures(1, &boxSelect.textureID);
    glDeleteTextures(1, &zoomIn.textureID);
    glDeleteTextures(1, &zoomOut.textureID);
    glDeleteTextures(1, &moveIcon.textureID);
    glDeleteTextures(1, &divisionSnapIcon.textureID);
    glDeleteTextures(1, &deleteIcon.textureID);
    glDeleteTextures(1, &editIcon.textureID);
    glDeleteTextures(1, &cutIcon.textureID);
    glDeleteTextures(1, &seekStart.textureID);
    glDeleteTextures(1, &seekEnd.textureID);

}
