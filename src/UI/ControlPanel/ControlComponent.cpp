//
// Created by nathan on 20/04/2026.
//
#include "ControlComponent.h"
#include "../../Singletons/PlayBackManager.h"

void ControlComponent::playbackControls() {
    static const float timeSliderWidth = 200.0f;
    playbackButtons.create();

    if (ImGui::BeginTable("##seek&time", 2, ImGuiTableFlags_SizingFixedFit, ImVec2(300, 80))) {
        ImGui::TableSetupColumn("##projectTimeline", ImGuiTableColumnFlags_WidthFixed, timeSliderWidth);
        ImGui::TableSetupColumn("##time");
        ImGui::TableNextColumn();
        //ImGui::Dummy(ImVec2(200, 80));
        auto lastTick = ArrangerManager::instance().getlastClipEndTime();
        auto currentTick = static_cast<uint32_t>(PlayBackManager::instance().getArrangerPlayheadTicks());
        float songPosition = std::clamp((timeSliderWidth/ static_cast<float>(lastTick)) * currentTick , 0.0f , timeSliderWidth);

        auto cursor = ImGui::GetCursorPos();
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(cursor.x,cursor.y),
            ImVec2(cursor.x + 200,cursor.y + 30),
                Theme::cTheme.backgroundAltPacked,
                3.0f
            );
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(cursor.x,cursor.y),
            ImVec2(cursor.x + songPosition,cursor.y + 30),
                Theme::cTheme.accentPacked,
                3.0f
            );
        ImGui::TableNextColumn();
        int endTimeSeconds = static_cast<int>(TimeData::msPerTick()* lastTick/1000) % 60;
        int endMinutes = std::floor(static_cast<int>(TimeData::msPerTick()* lastTick/1000) / 60.0f);
        std::string endSeconds = (endTimeSeconds< 10) ? "0"+std::to_string(endTimeSeconds):std::to_string(endTimeSeconds);

        int currentTimeSeconds = static_cast<int>(TimeData::msPerTick()* currentTick/1000) % 60;
        int currentMinutes = std::floor(static_cast<int>(TimeData::msPerTick()* currentTick/1000) / 60.0f);
        std::string currentSeconds = (currentTimeSeconds< 10) ? "0"+std::to_string(currentTimeSeconds):std::to_string(currentTimeSeconds);

        std::string endTime = std::to_string(endMinutes)+":"+ endSeconds;

        std::string currentTime = std::to_string(currentMinutes)+":"+ currentSeconds;
        std::string time = currentTime + "/" + endTime;
        ImGui::PushFont(Theme::monoSmall);
        ImGui::Text(time.c_str());
        ImGui::PopFont();
    }ImGui::EndTable();
}