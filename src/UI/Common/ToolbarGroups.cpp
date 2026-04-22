#include  "ToolbarGroups.h"
#include "../../Singletons/PlayBackManager.h"
#include "../../Singletons/PatternManager.h"


PlayBack ToolbarGroups::playback(bool isArranger, uint32_t ID) {
    return {
        "playback",
        [isArranger,ID](bool,uint32_t) {
            void* icon = !PlayBackManager::instance().isPlaying()
                ? (void*)(intptr_t)ASSETS.playIcon.textureID
                : (void*)(intptr_t)ASSETS.pauseIcon.textureID;
            if (isArranger) {PlayBackManager::instance().setMode(PlaybackMode::Arranger);}
            else {PlayBackManager::instance().setMode(PlaybackMode::PianoRoll);PatternManager::instance().setActivePatternID(ID);}
            if (ImGui::ImageButton("play_button", icon, ImVec2(32,32))){
                PlayBackManager::instance().togglePlay();
            }
        }

    };
}

PlayBack ToolbarGroups::seekStart(bool isArranger, uint32_t ID) {
    return {
        "seekStart",
        [isArranger,ID](bool,uint32_t) {
            void* icon = (void*)(intptr_t)ASSETS.seekStart.textureID;
            if (isArranger) {PlayBackManager::instance().setMode(PlaybackMode::Arranger);}
            else {
                PatternManager::instance().setActivePatternID(ID);
                PlayBackManager::instance().setMode(PlaybackMode::PianoRoll);
            }
            if (ImGui::ImageButton("seekStart", icon, ImVec2(32,32))){
                PlayBackManager::instance().pause();
                PlayBackManager::instance().seekToTicks(0, isArranger);
            }
        }
    };
}

PlayBack ToolbarGroups::seekEnd(bool isArranger, uint32_t ID) {
    return {
        "seekEnd",
        [isArranger,ID](bool,uint32_t) {
            void* icon = (void*)(intptr_t)ASSETS.seekEnd.textureID;
            if (isArranger) {PlayBackManager::instance().setMode(PlaybackMode::Arranger);}
            else {
                PatternManager::instance().setActivePatternID(ID);
                PlayBackManager::instance().setMode(PlaybackMode::PianoRoll);
            }
            if (ImGui::ImageButton("seekEnd", icon, ImVec2(32,32))){
                PlayBackManager::instance().pause();
                PlayBackManager::instance().seekToTicks(
                    ArrangerManager::instance().getlastClipEndTime(),
                    isArranger);
            }
        }

    };
}