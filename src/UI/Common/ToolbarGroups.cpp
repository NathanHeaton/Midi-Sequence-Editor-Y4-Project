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
            else {PlayBackManager::instance().setMode(PlaybackMode::PianoRoll);}

            if (ImGui::ImageButton("play_button", icon, ImVec2(32,32))){
                PatternManager::instance().setActivePatternID(ID);
                PlayBackManager::instance().togglePlay();
            }
        }

    };
}