
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
    uint32_t m_patternID = 0;
    float m_height = 0.0f;
    float m_velocityIncrement = 0.0f;

    void create(float &scrollX, uint32_t patternID) {
        m_patternID = patternID;
        m_height = ImGui::GetContentRegionAvail().y;
        m_velocityIncrement = m_height / 128;
        auto pattern = PatternManager::instance().getPatternByID(patternID);
        float lengthX = view_state->getPixelPerBar(zoomFactor::pianoRoll) * (pattern->m_bars + 1);
        if (ImGui::BeginChild("velocity window", ImVec2(0, 0),
            ImGuiChildFlags_None, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

            TimelineContext ctx;

            renderSteps(ctx);
            HandleMouseInput(ctx);
            
            ImGui::SetScrollX(scrollX);
            ImGui::Dummy(ImVec2(lengthX, 0));
            }
        ImGui::EndChild();
    }

private:

    void renderSteps(const TimelineContext& ctx) {
        DrawBars(ctx);
        renderPattern(ctx);;
    }

    void HandleMouseInput(const TimelineContext& ctx) {
        int absoluteTime = static_cast<int>(
        ctx.relativeX / ViewState::instance().getPixelPerBeat(pianoRoll) * TimeData::PPQ);
        uint8_t velocity = std::clamp(127 - static_cast<int>(ctx.relativeY / m_velocityIncrement), 0, 127);
        NoteCoordinate hoverCoord{velocity, static_cast<uint32_t>(absoluteTime)};


        if (ImGui::IsMouseDown(ImGuiMouseButton_Left)&& ImGui::IsWindowHovered()) {
            auto* pattern = PatternManager::instance().getPatternByID(m_patternID);
            for (auto noteIDs : pattern->m_noteEvents) {
                auto onEvent = pattern->getMidiEventByID_ptr(noteIDs.onID);
                if (pattern->m_hiddenNoteOnIDs.contains(noteIDs.onID)) {
                    continue;
                }
                auto offEvent = pattern->getMidiEventByID_ptr(noteIDs.offID);
                if ( hoverCoord.absoluteTime <= offEvent->m_absoluteTime &&
                    hoverCoord.absoluteTime >= onEvent->m_absoluteTime ) {
                    PatternManager::instance().setNoteVelocity(m_patternID, noteIDs ,velocity);
                }
            }
        }
    }

    void renderPattern(const TimelineContext& ctx) {
        auto* pattern = PatternManager::instance().getPatternByID(m_patternID);

        for (auto noteIDs : pattern->m_noteEvents) {
            auto onEvent = pattern->getMidiEventByID_ptr(noteIDs.onID);

            if (pattern->m_hiddenNoteOnIDs.contains(noteIDs.onID)) {
                continue;
            }
            auto offEvent = pattern->getMidiEventByID_ptr(noteIDs.offID);
            float startDelta =0;
            if (onEvent->m_absoluteTime != 0){startDelta =
                static_cast<float>(onEvent->m_absoluteTime) / TimeData::PPQ;}

            float endDelta=0;
            if (offEvent->m_absoluteTime != 0){endDelta =
                static_cast<float>(offEvent->m_absoluteTime) / TimeData::PPQ;}

            float startPixel = view_state->getPixelPerBeat(pianoRoll) * startDelta;
            float endPixel = view_state->getPixelPerBeat(pianoRoll) * endDelta ;
            float yStart = ctx.cursorPos.y + (m_height - (onEvent->getVelocity()+1)*m_velocityIncrement);
            float xStart = ctx.cursorPos.x + startPixel;
            float xEnd =  ctx.cursorPos.x + endPixel;

            auto colour = Theme::cTheme.barColourPacked;
            for (auto id : pattern->m_selectedNoteOnIDs) {
                if (noteIDs.onID == id) {
                    colour = Theme::cTheme.beatColourPacked;
                    break;
                }
            }
            ctx.drawList->AddLine(
                ImVec2(xStart, yStart),
                ImVec2(xStart, m_height + ctx.cursorPos.y),
                colour,2.0f
                );
            ctx.drawList->AddLine(
                ImVec2(xStart, yStart),
                ImVec2(xEnd,yStart),
                    colour, 3.0f);
        }
    }

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

            ImU32 colour = Theme::cTheme.beatColourPacked;
            if (checkIfBarStart(i)){colour=Theme::cTheme.barColourPacked;}

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