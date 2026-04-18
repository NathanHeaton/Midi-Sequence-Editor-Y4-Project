//
// Created by idad on 10/02/2026.
//
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "PatternManager.h"
#include "TimeData.h"

struct PatternClip{
    size_t patternID;
    uint32_t startTime;
    uint32_t endTime;
    size_t trackIndex;
    uint32_t ID;
    bool enabled = true;
};

struct Track {
    std::string title    = "Untitled Track";
    float volume         = 1.0f;
    float previousVolume = 1.0f;
    bool muted           = false;
    bool solo            = false;
    uint32_t instrumentID = 0;
};

struct ClipResizeOperation {
    bool isActive = false;
    uint32_t clipID = 0;

    void begin(uint32_t id) {
        isActive = true;
        clipID = id;
    }
    void reset() { isActive = false; clipID = 0; }
};

struct ClipMoveOperation {
    bool isActive = false;
    uint32_t clipID = 0;
    ArrangerCoordinate currentCoord{0,0};
    uint32_t mouseOffset{0};

    void begin(uint32_t id, ArrangerCoordinate inputCoord, uint32_t clipStartTime) {
        isActive = true;
        clipID = id;
        mouseOffset = inputCoord.time - clipStartTime;
        currentCoord = {setTime(inputCoord.time), inputCoord.track};
    }
    void update(ArrangerCoordinate coord)
    { currentCoord = {setTime(coord.time), coord.track};}
    void reset() { isActive = false; clipID = 0; }

private:
    uint32_t setTime(uint32_t time) {
        int newTime =  static_cast<signed>(time - mouseOffset);
        if (newTime<= 0) newTime = 0;
        return static_cast<uint32_t>(newTime);
    }
};

struct ClipIDHoverState {
    uint32_t ID;
    HoverState hoverState{NoHover};
};

class ArrangerManager
{

public:
    ClipMoveOperation moveOperation;
    ClipResizeOperation resizeOperation;
    std::vector<Track> tracks;
    std::vector<PatternClip> patternClips;

    [[nodiscard]] int getTrackAmount() const { return tracks.size(); }
    [[nodiscard]] Track* getTrack(size_t index) {
        if (index < tracks.size()) return &tracks[index];
        else return nullptr;
    }

    void addTrack() {tracks.push_back(Track());}
    [[nodiscard]] const std::vector<PatternClip>* const getPatternClips(){return &patternClips;}

    std::vector<Track>& getTracks(){return tracks;}

    static ArrangerManager& instance(){
        static ArrangerManager singleton;
        return singleton;
    }

    [[nodiscard]] PatternClip* getClipByID(uint32_t id) {
        for (auto& clips : patternClips) {
            if (clips.ID == id) return &clips;
        }
        return nullptr;
    }
    [[nodiscard]]  size_t getIndexFromID(uint32_t id) const {
        for (auto i{0u}; i < patternClips.size(); i++) {
            if (patternClips[i].ID == id) return i;
        }
        return SIZE_MAX;
    }

    [[nodiscard]] uint32_t getlastClipEndTime() {
        uint32_t lastClipEndTime = 0;
        for (auto clip : patternClips) {
            if (clip.endTime > lastClipEndTime)
            lastClipEndTime = clip.endTime;
        }
        return lastClipEndTime;
    }

    [[nodiscard]] PatternClip* getClipByIndex(size_t i){
        if (i < patternClips.size()) return &patternClips[i];
        else return nullptr;
    }

    void removeClip(ArrangerCoordinate pos) {
        for (auto clip : patternClips) {
            if ((clip.startTime <= pos.time && clip.endTime >= pos.time) && clip.trackIndex == pos.track) {
                std::cout<<clip.ID<<std::endl;

                patternClips.erase(patternClips.begin() + static_cast<signed>(getIndexFromID(clip.ID)));
                return;
            }
        }
    }

    ClipIDHoverState resolveHoverState(ArrangerCoordinate pos) {
        for (auto clips : patternClips) {
            if (clips.startTime <= pos.time && clips.endTime >= pos.time && clips.trackIndex == pos.track) {
                if (( clips.endTime - clips.startTime ) * 0.8 < pos.time - clips.startTime ) {
                    return {clips.ID, EdgeHover};
                }
                return {clips.ID, CenterHover};
            }
        }
        return {};
    }

    void addClip(ArrangerCoordinate pos) {
        if (moveOperation.isActive || resizeOperation.isActive) {return;}
        const auto p = PatternManager::instance().getPatternByID(PatternManager::instance().getActivePatternID());;
        PatternClip clip(p->ID,
            pos.time,pos.time +
            (p->m_bars * TimeData::instance().timeSignature.getDenominator() * TimeData::PPQ),
            pos.track,
            assignID());
        patternClips.emplace_back(clip);
        arrangerUpdated = true;
    }

    void removeAllClipsOnTrack(size_t trackIndex);

    void moveClip(uint32_t id, ArrangerCoordinate pos) {
        auto* clip = getClipByID(id);
        if (!clip) return;
        uint32_t duration = clip->endTime - clip->startTime;
        clip->startTime = pos.time;
        clip->endTime   = pos.time + duration;
        clip->trackIndex     = pos.track;
        arrangerUpdated = true;
    }

    void resizeClip(uint32_t id, uint32_t endTime) {
        auto* clip = getClipByID(id);
        if (!clip) return;
        if (clip->startTime >= endTime) {
            endTime = clip->endTime;
        }
        clip->endTime   = endTime;
        arrangerUpdated = true;
    }

    void trimClip(uint32_t id, uint32_t newStartTime, uint32_t newEndTime);

    void duplicateClip(uint32_t id, uint32_t newStartTime);
    void splitClip(uint32_t id, uint32_t splitTime);

    // track methods
    void setTrackInstrumentID(size_t index, uint32_t InstrumentID) {
        if (index < tracks.size()) tracks[index].instrumentID = InstrumentID;
    }
    void setTrackMuted(size_t index, bool muted) {
        if (index < tracks.size()) tracks[index].muted = muted;
    }
    void toggleTrackSolo(size_t index) {
        if (index >= tracks.size()) return;
        if (tracks[index].solo){
            for (auto& tracks : tracks) {
                tracks.muted = false;
                tracks.solo = false;
            }
        }
        else {
            for (auto& tracks : ArrangerManager::instance().getTracks()) {
                tracks.muted = true;
                tracks.solo = false;
            }
            tracks[index].solo = true;
            tracks[index].muted = false;
        }
    }

    void setTrackVolume(size_t index, float vol) {
        if (index < tracks.size()) tracks[index].volume = vol;
    }

    bool arrangerUpdated = false;

    private:
    uint32_t nextID{0};
    [[nodiscard]] uint32_t assignID(){return nextID++;}
    [[nodiscard]] size_t getTrackIndex(size_t trackIndex, uint32_t time);

    ArrangerManager() = default;
};