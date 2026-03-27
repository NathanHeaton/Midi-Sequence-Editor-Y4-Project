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
#include "Panels/velocity.h"
#include  "Panels/velocityLabel.h"

class PianoRollMain : public juce::Component
{
public:
    Tools toolbar;
    TimelineLabel timelineLabel;
    Piano piano;
    PianoRollComponent pianoRoll;
    Velocity velocity;
    VelocityLabels velocityLabels;
    float timelineXScroll = 0.0f;
    float timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::pianoRoll) * 8;

    PianoRollMain() = default;

    float pianoRollScrollY = 300.0f;
    float pianoRollScrollX;
    bool initialLoad = true;
    float velocityHeight      = 120.0f;
    float defaultVelocityHeight = 120.0f;

void create() {
    if (ImGui::Begin("pianoRollComponent", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar)) {

        toolbar.create();

        if (ImGui::BeginTable("table", 2, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("gap", ImGuiTableColumnFlags_WidthFixed,
                ViewState::instance().getWhiteSize().x);
            ImGui::TableSetupColumn("timeline", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableNextColumn();
            ImGui::Dummy(ImVec2(ViewState::instance().getWhiteSize().x, 35));
            ImGui::TableNextColumn();
            if (ImGui::BeginChild("horizontalScroll", ImVec2(0, 15), false,
                ImGuiWindowFlags_HorizontalScrollbar)) {
                ImGui::Dummy(ImVec2(timelineLength, 15));
                timelineXScroll = ImGui::GetScrollX();
                } ImGui::EndChild();
            timelineLabel.create(timelineLength, timelineXScroll,
                PatternManager::instance().getCurrentPattern().m_bars,
                zoomFactor::pianoRoll);
        }
        ImGui::EndTable();

        float totalAvail    = ImGui::GetContentRegionAvail().y;
        float dragBarHeight = 6.0f;
        float minVelHeight  = 20.0f;

        velocityHeight = std::clamp(velocityHeight, minVelHeight, totalAvail - 60.0f);
        float pianoRollHeight = totalAvail - velocityHeight - dragBarHeight;

        if (ImGui::BeginTable("note_edit_elements", 3, ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupColumn("Piano");
            ImGui::TableSetupColumn("sequence grid", ImGuiTableColumnFlags_WidthStretch, 0);
            ImGui::TableSetupColumn("verticalScroll", ImGuiTableColumnFlags_WidthFixed, 15);

            ImGui::TableNextColumn();
            piano.create(pianoRollScrollY, pianoRollHeight);

            ImGui::TableNextColumn();
            pianoRoll.create(pianoRollScrollY, timelineXScroll, timelineLength, pianoRollHeight);

            ImGui::TableNextColumn();
            if (ImGui::BeginChild("custom_scroll", ImVec2(15, pianoRollHeight), false,
                ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
                ImGui::Dummy(ImVec2(15, ViewState::instance().WHITE_KEYS
                    * ViewState::instance().getWhiteSize().y));
                if (initialLoad) {
                    ImGui::SetScrollY(ViewState::instance().getNoteHeight() * 48);
                    initialLoad = false;
                }
                pianoRollScrollY = ImGui::GetScrollY();
            } ImGui::EndChild();
        } ImGui::EndTable();

        bool collapsed = (velocityHeight <= minVelHeight + 1.0f);
        // const char* arrow = "Velocity";
        // if (ImGui::Button(arrow, ImVec2(120, dragBarHeight + 8))) {
        //     velocityHeight = collapsed
        //         ? defaultVelocityHeight   // expand to default
        //         : minVelHeight;           // collapse
        // }
        // ImGui::SameLine();
        // ImGui::Button("##drag_handle",
        //     ImVec2(ImGui::GetContentRegionAvail().x, dragBarHeight + 8));
        // if (ImGui::IsItemActive()) {
        //     velocityHeight -= ImGui::GetIO().MouseDelta.y;  // drag up = more height
        //     velocityHeight = std::clamp(velocityHeight, minVelHeight, totalAvail - 60.0f);
        // }

        if (!collapsed) {
            if (ImGui::BeginTable("velocity_elements", 3, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Piano gap",
                    ImGuiTableColumnFlags_WidthFixed,
                    ViewState::instance().getWhiteSize().x);
                ImGui::TableSetupColumn("vel grid",
                    ImGuiTableColumnFlags_WidthStretch, 0);
                ImGui::TableSetupColumn("scroll gap",
                    ImGuiTableColumnFlags_WidthFixed, 15);

                ImGui::TableNextColumn();
                velocityLabels.create(velocityHeight);
                ImGui::TableNextColumn();
                velocity.create(timelineXScroll, timelineLength, velocityHeight);
                ImGui::TableNextColumn();
            } ImGui::EndTable();
        }
    }
    ImGui::End();
}

};
