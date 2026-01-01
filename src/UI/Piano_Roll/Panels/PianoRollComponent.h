//
// Created by nathan on 24/11/2025.
//
#pragma once

#include "../../../Theme.h"
#include "../../../SessionData.h"

//#include "PianoRoll/PianoKeys.h"

class PianoRollComponent
{
public:

    PianoRollComponent() = default;

    SessionData* s = &SessionData::instance();

    bool bg_tone = false;

    struct TimelineContext {
        ImVec2 cursorPos;
        ImDrawList* drawList;
        float height;
        float scrollX;
        float width;
        int firstVisibleBeat;
        int lastVisibleBeat;
        float barWidth;

        TimelineContext() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height =SessionData::instance().getWhiteSize().y * SessionData::instance().getWhiteKeys();
            width = ImGui::GetWindowWidth();
            scrollX = ImGui::GetScrollX();
            barWidth = 2 * SessionData::instance().timeSignature.getNumerator() * SessionData::instance().getPixelPerBeat();
            auto& session = SessionData::instance();
            firstVisibleBeat = scrollX != 0.0f ?
                static_cast<int>(scrollX / session.getPixelPerBeat()) : 0;
            lastVisibleBeat = static_cast<int>((scrollX + width) / session.getPixelPerBeat());
        }
    };

    void create(float &pianoRollScrollY) {
        s->getWhiteSize().y * s->getWhiteKeys();
        if (ImGui::BeginChild("piano grid",ImVec2(0,0),
            ImGuiChildFlags_None,
            ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {

            DrawBars();
            DrawOctaveLines();

            pianoRollScrollY = ImGui::GetScrollY();

            ImGui::Dummy(ImVec2(1000,1000));
        }
        ImGui::EndChild();

    }

    bool checkIfBarStart(int beat) {
        return beat % s->timeSignature.getNumerator() == 0;
    }

    void DrawBars() {
        TimelineContext ctx;

        int increment = s->timeSignature.getNumerator();
        increment = increment*2;

        getCurrentBarCount(ctx);
        DrawBarBackgrounds(ctx);
        for (int i = ctx.firstVisibleBeat; i <= ctx.lastVisibleBeat; i++) {
            bool barStart = checkIfBarStart(i);
            ImVec2 beatPosStart = ImVec2(
                ctx.cursorPos.x + i * s->getPixelPerBeat(),
                barStart ? ctx.cursorPos.y : ctx.cursorPos.y + 10
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

    void DrawBarBackgrounds(const TimelineContext& ctx) {
        int beatsPerBar = s->timeSignature.getNumerator();
        int beatsPerBackground = beatsPerBar * 2;
        int firstBackgroundBeat = (ctx.firstVisibleBeat / beatsPerBackground) * beatsPerBackground;
        int backgroundIndex = firstBackgroundBeat / beatsPerBackground;
        bg_tone = (backgroundIndex % 2) == 0;
        for (int beat = firstBackgroundBeat; beat <= ctx.lastVisibleBeat; beat += beatsPerBackground) {
            DrawBackground(ctx, beat);
            bg_tone = !bg_tone;
        }
    }

    void DrawBackground(const TimelineContext& ctx, int startBeat) {
        ImVec2 rectStart = ImVec2(
            ctx.cursorPos.x + s->getPixelPerBeat() * startBeat,
            ctx.cursorPos.y + 10
        );
        ImVec2 rectEnd = ImVec2(
            rectStart.x + ctx.barWidth,
            ctx.cursorPos.y + ctx.height
        );
        ctx.drawList->AddRectFilled(
            rectStart, rectEnd,
            bg_tone ? Theme::currentThemeColours.backgroundAltPacked
                   : Theme::currentThemeColours.backgroundPacked,
            0.0f
        );
    }

    void DrawBarLine(const TimelineContext& ctx, ImVec2 start, ImVec2 end, bool barStart) {
        ctx.drawList->AddLine(
            start, end,
            barStart ? Theme::currentThemeColours.barColourPacked
                     : Theme::currentThemeColours.beatColourPacked,
            1.0f
        );
    }


    void DrawOctaveLines() {
        TimelineContext ctx;
        float octaveHeight = s->getWhiteSize().y *7;
        int octaves = 7;
        for (unsigned int i = 0; i < octaves; i++) {
            float yPos = ctx.cursorPos.y + 10 + i * octaveHeight;

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.barColourPacked,
                1.0f
            );
        }
    }


private:

};
