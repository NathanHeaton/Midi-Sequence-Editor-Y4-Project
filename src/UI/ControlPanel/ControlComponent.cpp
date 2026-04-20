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
        int endTimeSeconds = static_cast<int>(TimeData::instance().msPerTick()* lastTick/1000) % 60;
        int endMinutes = std::floor(static_cast<int>(TimeData::instance().msPerTick()* lastTick/1000) / 60.0f);
        std::string endSeconds = (endTimeSeconds< 10) ? "0"+std::to_string(endTimeSeconds):std::to_string(endTimeSeconds);

        int currentTimeSeconds = static_cast<int>(TimeData::instance().msPerTick()* currentTick/1000) % 60;
        int currentMinutes = std::floor(static_cast<int>(TimeData::instance().msPerTick()* currentTick/1000) / 60.0f);
        std::string currentSeconds = (currentTimeSeconds< 10) ? "0"+std::to_string(currentTimeSeconds):std::to_string(currentTimeSeconds);

        std::string endTime = std::to_string(endMinutes)+":"+ endSeconds;

        std::string currentTime = std::to_string(currentMinutes)+":"+ currentSeconds;
        std::string time = currentTime + "/" + endTime;
        ImGui::PushFont(Theme::monoSmall);
        ImGui::Text(time.c_str());
        ImGui::PopFont();
    }ImGui::EndTable();
}

void ControlComponent::projectDataControls()
{
    double bpm = TimeData::instance().getBPM();
    ImGui::PushFont(Theme::textSmall);
    if (ImGui::InputDouble("BPM", &bpm, 1, 5, "%.1f", ImGuiInputTextFlags_CharsDecimal)) {
        ImGui::PopFont();
        ImGui::PushFont(Theme::monoSmall);
        bpm = std::clamp(bpm, 1.0, 999.0);
        TimeData::instance().setBPM(bpm);
        ImGui::PopFont();
    } else ImGui::PopFont();

    auto num = static_cast<signed>(TimeData::instance().timeSignature.getNumerator());
    auto den = static_cast<signed>(TimeData::instance().timeSignature.getDenominator());
    ImGui::Separator();
    ImGui::PushFont(Theme::textSmall);
    ImGui::Text("Time Signature: ");
    ImGui::PopFont();

    if ( ImGui::BeginTable("##timeSig",3,ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableSetupColumn("##num",0,30.f);
        ImGui::TableSetupColumn("##/");
        ImGui::TableSetupColumn("##den",0,30.f);
        ImGui::TableNextColumn();
        ImGui::PushFont(Theme::monoSmall);
        if (ImGui::InputInt("##numerator",&num,0,0,ImGuiInputTextFlags_CharsDecimal))
        {
            num = std::clamp(num, 1, 100);
            TimeData::instance().timeSignature.change(num,den);
        }
        ImGui::TableNextColumn();
        ImGui::Text("/");
        ImGui::TableNextColumn();
        if (ImGui::InputInt("##Denominator",&den,0,0,ImGuiInputTextFlags_CharsDecimal))
        {
            den = std::clamp(den, 1, 100);
            TimeData::instance().timeSignature.change(num,den);
        }
        ImGui::PopFont();
    }ImGui::EndTable();
}
