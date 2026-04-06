#include  "ToolbarGroups.h"
#include "../../Singletons/PlayBackManager.h"


ToolbarGroup ToolbarGroups::playback() {
    return {
        "playback",
        [](bool) {
            void* icon = !PlayBackManager::instance().isPlaying()
                ? (void*)(intptr_t)ASSETS.playIcon.textureID
                : (void*)(intptr_t)ASSETS.pauseIcon.textureID;

            if (ImGui::ImageButton("play_button", icon, ImVec2(32,32)))
                PlayBackManager::instance().togglePlay();
        }

    };
}