//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../Singletons/SessionData.h"
#include "../../../Singletons/PatternManager.h"
#include "../../../Singletons/ToolManager.h"

class PianoRollComponent
{
public:
    PianoRollComponent() = default;
    SessionData* s = &SessionData::instance();
    bool bg_tone = false;

    void create(float &scrollY,float &scrollX, float& lengthX) {
        if (ImGui::BeginChild("piano grid",ImVec2(0,0),
            ImGuiChildFlags_None,ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse)) {
            TimelineContext ctx;

            renderSteps(ctx);
            HandleMouseInput(ctx);
            HandleKeyboardInput(ctx);

            ImGui::SetScrollX(scrollX);
            ImGui::SetScrollY(scrollY);
            auto& pattern = PatternManager::instance().getCurrentPattern();
            lengthX =  SessionData::instance().getPixelPerBar(zoomFactor::pianoRoll) * pattern.m_bars;
            ImGui::Dummy(ImVec2(lengthX ,s->getWhiteKeys()*s->getWhiteSize().y));
        }
        ImGui::EndChild();
    }



private:
    const int octaves = 10;
    struct TimelineContext {
        ImVec2 cursorPos;
        ImDrawList* drawList;
        float height;
        float scrollX;
        float scrollY;
        float width;
        int firstVisibleBeat;
        int lastVisibleBeat;
        float barWidth;
        float noteHeight;
        float intialGap;
        float relativeX;
        float relativeY;
        ToolTypes activeTool;

        TimelineContext() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height =SessionData::instance().getWhiteSize().y * SessionData::instance().getWhiteKeys();
            width = ImGui::GetWindowWidth();
            scrollX = ImGui::GetScrollX();
            scrollY = ImGui::GetScrollY();
            barWidth = 2 * SessionData::instance().timeSignature.getNumerator() * SessionData::instance().getPixelPerBar(zoomFactor::pianoRoll);
            noteHeight = SessionData::instance().getNoteHeight();
            intialGap = (SessionData::instance().getWhiteSize().y * 5.0f)/8.0f;
            auto& session = SessionData::instance();
            firstVisibleBeat = scrollX != 0.0f ?
                static_cast<int>(scrollX / session.getPixelPerBeat(zoomFactor::pianoRoll)) : 0;
            lastVisibleBeat = static_cast<int>((scrollX + width) / session.getPixelPerBeat(zoomFactor::pianoRoll));
            ImVec2 mousePos = ImGui::GetMousePos();
            relativeX = mousePos.x - cursorPos.x;
            relativeY = mousePos.y - cursorPos.y;
            activeTool = ToolManager::instance().getActiveNoteTool();
        }
    };

    void renderSteps(const TimelineContext& ctx) {
        DrawNoteGuides(ctx);
        DrawBars(ctx);
        DrawOctaveLines(ctx);
        renderPattern(ctx);
        DrawToolEffects(ctx);
    }

    void HandleMouseInput(const TimelineContext& ctx);
    void renderPattern(const TimelineContext& ctx);
    void DrawToolEffects(const TimelineContext& ctx);
    void HandleKeyboardInput(const TimelineContext& ctx);

    bool checkIfBarStart(int beat) {return beat % s->timeSignature.getNumerator() == 0;}

    void DrawBars(auto& ctx) {
        int increment = s->timeSignature.getNumerator();
        increment = increment*2;
        getCurrentBarCount(ctx);
        for (int i = ctx.firstVisibleBeat; i <= ctx.lastVisibleBeat; i++) {
            bool barStart = checkIfBarStart(i);
            ImVec2 beatPosStart = ImVec2(
                ctx.cursorPos.x + i * s->getPixelPerBeat(zoomFactor::pianoRoll),
                barStart ? ctx.cursorPos.y : ctx.cursorPos.y
            );
            ImVec2 beatPosEnd = ImVec2(beatPosStart.x, ctx.cursorPos.y + ctx.height);

            DrawBarLine(ctx, beatPosStart, beatPosEnd, barStart);
        }
    }

    void getCurrentBarCount(const TimelineContext& ctx) {
        int increment = s->timeSignature.getNumerator();
        increment = increment*2;
        if (ctx.firstVisibleBeat % increment == 0) {
            bg_tone = !bg_tone;
        }
    }

    void DrawBarLine(const TimelineContext& ctx, ImVec2 start, ImVec2 end, bool barStart) {
        ctx.drawList->AddLine(
            start, end,
            barStart ? Theme::currentThemeColours.barColourPacked
                     : Theme::currentThemeColours.beatColourPacked,
            1.0f
        );
    }

    void DrawOctaveLines(const TimelineContext& ctx) {
        float octaveHeight = s->getWhiteSize().y *7;
        for (unsigned int i = 0; i < octaves; i++) {
            float yPos = ctx.cursorPos.y + i * octaveHeight + ( s->getWhiteSize().y*5);

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.barColourPacked,
                1.0f
            );
        }
    }

    void DrawNoteGuides(const TimelineContext& ctx) {
        float noteGap = (s->getWhiteSize().y *7.0f)/12.0f;
        int notes = 128;
        bool whiteNote = true;
        int octaveNoteIndex = 0;
        int noteOffset = 5;
        bool firstNote = true;


        for (auto i{0u}; i < notes; i++) {
            float yPos;
            if (i < 8) {
                yPos = ctx.cursorPos.y + i * ctx.intialGap;
            }
            else {
                yPos = (ctx.cursorPos.y + (i-8) * noteGap) + ctx.intialGap *8;
            }
            ctx.drawList->AddRectFilled(ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.cursorPos.x+ ctx.width, yPos + noteGap ),
                whiteNote ? Theme::currentThemeColours.backgroundAltPacked : Theme::currentThemeColours.backgroundPacked);

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.beatColourPacked,
                1.0f
            );
            if (((octaveNoteIndex+noteOffset)% 12 < 7 || (octaveNoteIndex+noteOffset)%12 > 7) && (octaveNoteIndex+noteOffset)% 12 != 0 ) {
                whiteNote= !whiteNote;
            }
            octaveNoteIndex++;
            firstNote = false;
        }
    }
};
