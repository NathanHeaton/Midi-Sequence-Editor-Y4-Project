//
// Created by nathan on 09/12/2025.
//
#pragma once
#include "imgui.h"
#include "../../Singletons/SessionData.h"
#include "../../Theme.h"

class Timeline {
public:
    Timeline() = default;
    SessionData* s = &SessionData::instance();

    bool bg_tone{true};
    int barBackgroundCount{0};

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
            height = ImGui::GetWindowHeight();
            width = ImGui::GetWindowWidth();
            scrollX = ImGui::GetScrollX();
            barWidth = SessionData::instance().getPixelPerBar(zoomFactor::arranger);
            auto& session = SessionData::instance();
            firstVisibleBeat = scrollX != 0.0f ?
                static_cast<int>(scrollX / session.getPixelPer(Division::QUARTER_NOTE, zoomFactor::arranger)) : 0;
            lastVisibleBeat = static_cast<int>((scrollX + width) / session.getPixelPerBeat(zoomFactor::arranger));
        }
    };


    void createTimeline(float &timelineLength, float &xScroll) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::BeginChild("Timeline", ImVec2(0, s->getTrackHeight() * s->getTrackAmount()),
            false,
            ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
            DrawBars();
            DrawTrackSeparator();
            if (timelineLength < SessionData::instance().getPixelPerBar(zoomFactor::arranger)* 60) {
                if (ImGui::GetScrollMaxX() == ImGui::GetScrollX()) {
                    s->setTotalBars(s->getTotalBars()+ 4);
                    timelineLength = s->getTotalBars() * s->getPixelPerBar(zoomFactor::arranger);
                }
            }
            xScroll = ImGui::GetScrollX();
            ImGui::Dummy(ImVec2(timelineLength, s->getTrackHeight() * s->getTrackAmount()));

        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
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
                ctx.cursorPos.x + i * s->getPixelPerBeat(zoomFactor::arranger),
                ctx.cursorPos.y
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
        int beatsPerBackground = s->timeSignature.getNumerator() * 2;
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
            ctx.cursorPos.x + s->getPixelPerBeat(zoomFactor::arranger) * startBeat,
            ctx.cursorPos.y
        );
        ImVec2 rectEnd = ImVec2(
            rectStart.x + s->getPixelPerBar(zoomFactor::arranger)*2,
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


    void DrawTrackSeparator() {
        TimelineContext ctx;

        for (unsigned int i = 0; i < s->getTrackAmount(); i++) {
            float yPos = ctx.cursorPos.y + i * s->getTrackHeight();

            ctx.drawList->AddLine(
                ImVec2(ctx.scrollX + ctx.cursorPos.x, yPos),
                ImVec2(ctx.scrollX + ctx.width + ctx.cursorPos.x, yPos),
                Theme::currentThemeColours.barColourPacked,
                1.0f
            );
        }
    }
};