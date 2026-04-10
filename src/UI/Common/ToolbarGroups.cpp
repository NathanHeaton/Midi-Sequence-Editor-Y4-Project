#include  "ToolbarGroups.h"
#include "../../Singletons/PlayBackManager.h"


ToolbarGroup ToolbarGroups::playback(bool isArranger) {
    return {
        "playback",
        [isArranger](bool) {
            void* icon = !PlayBackManager::instance().isPlaying()
                ? (void*)(intptr_t)ASSETS.playIcon.textureID
                : (void*)(intptr_t)ASSETS.pauseIcon.textureID;
            if (isArranger) {PlayBackManager::instance().setMode(PlaybackMode::Arranger);}
            else {PlayBackManager::instance().setMode(PlaybackMode::PianoRoll);}

            if (ImGui::ImageButton("play_button", icon, ImVec2(32,32)))
                PlayBackManager::instance().togglePlay();
        }

    };
}