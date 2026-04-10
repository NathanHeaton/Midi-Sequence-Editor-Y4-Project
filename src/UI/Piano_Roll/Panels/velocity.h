
//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../Singletons/ViewState.h"
#include "../../../Singletons/PatternManager.h"
#include "../../../Singletons/ToolManager.h"
#include "TimelineContext.h"

class Velocity {
public:

    Velocity() = default;
    ViewState* view_state = &ViewState::instance();
    bool bg_tone = false;

    void create(float &scrollX) {
        auto pattern = PatternManager::instance().getCurrentPattern();
        float lengthX = view_state->getPixelPerBar(zoomFactor::pianoRoll) * pattern->m_bars;
        if (ImGui::BeginChild("velocity window", ImVec2(lengthX, 0),
            ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

            ImGui::Dummy(ImVec2(lengthX, 0));
            TimelineContext ctx;
            renderSteps(ctx);
            // test
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetCursorPos(),
                          ImGui::GetContentRegionAvail(),Theme::currentThemeColours.accentPacked
                          );

            ImGui::SetScrollX(scrollX);

            }
        ImGui::EndChild();
    }

private:
    const int octaves = 10;

    void renderSteps(const TimelineContext& ctx) {
        DrawBars(ctx);
    //    renderPattern(ctx);;
    }

    void HandleMouseInput(const TimelineContext& ctx);
    void renderPattern(const TimelineContext& ctx) const;

    bool checkIfBarStart(const int beat) const {return beat % (TimeData::instance().timeSignature.getNumerator() * view_state->getRenderedSubDivisions()) == 0;}

    void DrawBars(auto& ctx) {
        int increment = TimeData::instance().timeSignature.getNumerator();
        increment = increment*2;
        getCurrentBarCount(ctx);
        for (int i = ctx.firstVisibleSubBeat; i <= ctx.lastVisibleSubBeat; i++) {
            ImVec2 beatPosStart = ImVec2(
                ctx.cursorPos.x + i * view_state->getPixelPerBeat(zoomFactor::pianoRoll)/view_state->getRenderedSubDivisions(),
                 ctx.cursorPos.y
            );
            ImVec2 beatPosEnd = ImVec2(beatPosStart.x, ctx.cursorPos.y + ctx.height);

            ImU32 colour = Theme::currentThemeColours.beatColourPacked;
            if (checkIfBarStart(i)){colour=Theme::currentThemeColours.barColourPacked;}

            DrawBarLine(ctx, beatPosStart, beatPosEnd, colour);
        }
    }

    void getCurrentBarCount(const TimelineContext& ctx) {
        int increment = TimeData::instance().timeSignature.getNumerator();
        increment = increment*2;
        if (ctx.firstVisibleSubBeat*view_state->getRenderedSubDivisions() % increment == 0) {
            bg_tone = !bg_tone;
        }
    }

    static void DrawBarLine(const TimelineContext& ctx, ImVec2 start, ImVec2 end, ImU32 colour) {
        ctx.drawList->AddLine(
            start, end, colour,1.0f
        );
    }
};