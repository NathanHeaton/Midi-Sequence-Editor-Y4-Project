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
    float timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::pianoRoll) * 8;

    PianoRollMain() = default;

    float pianoRollScrollY = 300.0f;
    float pianoRollScrollX;
    bool intialLoad = true;

    void create() {
        if (ImGui::Begin("pianoRollComponent", nullptr, ImGuiWindowFlags_NoMove)) {
            toolbar.create();
            if (ImGui::BeginTable("table", 2, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("gap");
                ImGui::TableSetupColumn("timeline", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableNextColumn();
                ImGui::Dummy(ImVec2(0,35));
                ImGui::TableNextColumn();
                if (ImGui::BeginChild("horizontalScroll", ImVec2(0, 15), false, ImGuiWindowFlags_HorizontalScrollbar)) {
                    ImGui::Dummy(ImVec2(timelineLength, 15));
                    timelineXScroll = ImGui::GetScrollX();
                }ImGui::EndChild();
                timelineLabel.create(timelineLength,timelineXScroll,
                PatternManager::instance().getCurrentPattern().m_bars,
                zoomFactor::pianoRoll);
            }
            ImGui::EndTable();

            if (ImGui::BeginTable("table", 3, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Piano");
                ImGui::TableSetupColumn("sequence grid", ImGuiTableColumnFlags_WidthStretch, 0);
                ImGui::TableSetupColumn("verticalScroll",ImGuiTableColumnFlags_WidthFixed,15);
                ImGui::TableNextColumn();
                piano.create(pianoRollScrollY);
                ImGui::TableNextColumn();
                pianoRoll.create(pianoRollScrollY,timelineXScroll, timelineLength);
                ImGui::TableNextColumn();

                if (ImGui::BeginChild("custom_scroll", ImVec2(15, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
                    ImGui::Dummy(ImVec2(15, ViewState::instance().WHITE_KEYS * ViewState::instance().getWhiteSize().y));
                    if (intialLoad)  ImGui::SetScrollY(ViewState::instance().getNoteHeight()* 48); intialLoad = false;
                    pianoRollScrollY = ImGui::GetScrollY();
                }ImGui::EndChild();
            }ImGui::EndTable();
        }ImGui::End();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollMain)
};
