//
// Created by idad on 10/02/2026.
//
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "PatternManager.h"
struct PatternClip{
    size_t patternIndex;
    uint32_t startTime;
    uint32_t endTime;
    uint32_t track;
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
    std::vector<PatternClip> patternClips;

    [[nodiscard]] int getTrackAmount() const { return tracks.size(); }
    [[nodiscard]] Track* getTrack(size_t index) {
        if (index < tracks.size()) return &tracks[index];
        else return nullptr;
    }

    void addTrack() {tracks.push_back(Track());}
    [[nodiscard]] const std::vector<PatternClip>* const getPatternClips(){return &patternClips;}

    static ArrangerManager& instance(){
        static ArrangerManager singleton;
        return singleton;
    }

    [[nodiscard]] std::vector<PatternClip*> getClipsOnTrack(size_t trackIndex);
    [[nodiscard]] std::vector<PatternClip*> getClipsInRange(uint32_t start, uint32_t end);
    [[nodiscard]] PatternClip* getClipByID(uint32_t id);
    [[nodiscard]] PatternClip* getClipByIndex(size_t i){
        if (i < patternClips.size()) return &patternClips[i];
        else return nullptr;
    }
    [[nodiscard]] PatternClip* getClipAtTime(size_t trackIndex, uint32_t time);
    [[nodiscard]] bool hasOverlap(size_t trackIndex, uint32_t start, uint32_t end, uint32_t excludeID = UINT32_MAX) const;

    void removeClip(ArrangerCoordinate pos) {
        for (auto clips : patternClips) {
            if (clips.startTime < pos.time && clips.endTime > pos.time && clips.track == pos.track) {
                patternClips.erase(patternClips.begin() + clips.startTime);
            }
        }
    }

    void addClip(ArrangerCoordinate pos) {
        PatternClip clip(PatternManager::instance().activePatternIndex,
            pos.time,pos.time + 960,
            pos.track,
            assignID());
        patternClips.push_back(clip);
    }

    void removeAllClipsOnTrack(size_t trackIndex);

    void moveClip(uint32_t id, size_t newTrackIndex, uint32_t newStartTime);
    void trimClip(uint32_t id, uint32_t newStartTime, uint32_t newEndTime);

    void duplicateClip(uint32_t id, uint32_t newStartTime);
    void splitClip(uint32_t id, uint32_t splitTime);

    private:
    uint32_t nextID{0};
    [[nodiscard]] uint32_t assignID(){return nextID++;}
    [[nodiscard]] size_t getTrackIndex(size_t trackIndex, uint32_t time);

    ArrangerManager() = default;
};