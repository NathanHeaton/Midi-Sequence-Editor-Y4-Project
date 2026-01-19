//
// Created by nathan on 24/11/2025.
//
#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../Theme.h"
#include "Panels/Tools.h"
#include "Panels/PianoRollComponent.h"
#include <iostream>
#include <imgui.h>
#include "Panels/piano.h"
#include "../Common/timelineLabel.h"

class PianoRollMain : public juce::Component
{
public:
    Tools toolbar;
    TimelineLabel timelineLabel;
    Piano piano;
    PianoRollComponent pianoRoll;

    float timelineXScroll = 0.0f;
    float timelineLength = SessionData::instance().getPixelPerBar(zoomFactor::pianoRoll) * 8;

    PianoRollMain() = default;

    float pianoRollScrollY;
    float pianoRollScrollX;

    void create() {
        if (ImGui::Begin("pianoRollComponent", nullptr)) {
            toolbar.create();
            if (ImGui::BeginTable("table", 2, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Piano");
                ImGui::TableSetupColumn("Piano", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableNextColumn();
                ImGui::Dummy(ImVec2(0,35));
                piano.create(pianoRollScrollY);
                ImGui::TableNextColumn();
                if (ImGui::BeginChild("custom_scroll", ImVec2(0, 15), false, ImGuiWindowFlags_HorizontalScrollbar)) {
                    ImGui::Dummy(ImVec2(timelineLength, 15));
                    timelineXScroll = ImGui::GetScrollX();
                }ImGui::EndChild();
                timelineLabel.create(timelineLength,timelineXScroll,
                    SessionData::instance().getTotalBarsPianoRoll(),
                    zoomFactor::pianoRoll);
                pianoRoll.create(pianoRollScrollY,timelineXScroll, timelineLength);
            }ImGui::EndTable();
        }ImGui::End();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollMain)
};
