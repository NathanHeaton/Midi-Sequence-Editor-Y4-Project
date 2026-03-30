//
// Created by idad on 10/02/2026.
//
#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct patternClip{
    size_t patternIndex;
    uint32_t startTime;
    uint32_t endTime;
    uint32_t ID;
};

struct Track{
    float volume=1.0f;
    std::string title = "placeholder";
    bool muted=false;
    bool solo=false;
    // TODO: type for instrument
};

class ArrangerManager
{
public:
    std::vector<Track> tracks;

    [[nodiscard]] int getTrackAmount() const { return tracks.size(); }
    [[nodiscard]] Track* getTrack(size_t index) {
        if (index < tracks.size()) return &tracks[index];
        else return nullptr;
    }

    void addTrack() {tracks.push_back(Track());}

    static ArrangerManager& instance(){
        static ArrangerManager singleton;
        return singleton;
    }


    private:
    ArrangerManager() = default;
};