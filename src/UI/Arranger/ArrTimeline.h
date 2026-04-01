//
// Created by nathan on 09/12/2025.
//
#pragma once
#include "imgui.h"
#include "../../Singletons/ViewState.h"
#include "../../Singletons/ProjectData.h"
#include "../../Theme.h"
#include "ArrTimelineHandleInput.h"
#include "ArrangerContext.h"

class ArrTimeline {
public:
    ArrTimeline() = default;
    ViewState* s = &ViewState::instance();
    ArrTimelineHandleInput timelineHandleInput;
    bool bg_tone{true};
    int barBackgroundCount{0};

    void createTimeline(float &timelineLength, float &xScroll) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        if (ImGui::BeginChild("Timeline", ImVec2(0, s->getTrackHeight() * ArrangerManager::instance().getTrackAmount()),
            false,
            ImGuiWindowFlags_NoScrollbar)) {
            ArrangerContext ctx;

            renderSteps(ctx);
            timelineHandleInput.process(ctx);
            if (timelineLength < s->getPixelPerBar(zoomFactor::arranger)* 60) {
                if (ImGui::GetScrollMaxX() == ImGui::GetScrollX()) {
                    ProjectData::instance().setTotalBars(ProjectData::instance().getTotalBars()+ 4);
                    timelineLength = ProjectData::instance().getTotalBars() * s->getPixelPerBar(zoomFactor::arranger);
                }
            }
            ImGui::SetScrollX(xScroll);
            ImGui::Dummy(ImVec2(timelineLength, s->getTrackHeight() * ArrangerManager::instance().getTrackAmount()));

        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
    }

    void renderSteps(const ArrangerContext& ctx)    {
        DrawBars(ctx);
        DrawTrackSeparator(ctx);
        renderPatternClips(ctx);
    }

    bool checkIfBarStart(int beat) {
        return beat % TimeData::instance().timeSignature.getNumerator() == 0;
    }

    void DrawBars(const ArrangerContext& ctx) {
        int increment = TimeData::instance().timeSignature.getNumerator();
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

    void getCurrentBarCount(const ArrangerContext& ctx) {
        int increment = TimeData::instance().timeSignature.getNumerator();
        increment = increment*2;
        if (ctx.firstVisibleBeat % increment == 0) {
            bg_tone = !bg_tone;
        }

    }

    void renderPatternClips(const ArrangerContext& ctx){
        auto clips = ArrangerManager::instance().getPatternClips();
        for (auto clip : *clips){
            ImVec2 p1((clip.startTime/TimeData::PPQ)*ViewState::instance().getPixelPerBeat(arranger)+ctx.cursorPos.x,
                clip.track* ViewState::instance().getTrackHeight()+ctx.cursorPos.y);
            ImVec2 p2((clip.endTime/TimeData::PPQ)*ViewState::instance().getPixelPerBeat(arranger)+ctx.cursorPos.x,clip.track * ViewState::instance().getTrackHeight()
                +ctx.cursorPos.y+ViewState::instance().getTrackHeight());

            ctx.drawList->AddRectFilledMultiColor(p1,p2,
                Theme::currentThemeColours.patternClip.col0,
                Theme::currentThemeColours.patternClip.col0,
                Theme::currentThemeColours.patternClip.col1,
                Theme::currentThemeColours.patternClip.col1
                );
            ctx.drawList->AddRect(p1,p2,
            Theme::currentThemeColours.barColourPacked,
            3.0f,
            0,
            1.0f
            );
        auto pattern = PatternManager::instance().getPatternByID(clip.ID);
        ctx.drawList->AddText(p1 ,Theme::currentThemeColours.barColourPacked,
            pattern->m_title.c_str());
        }
    }

    void DrawBarBackgrounds(const ArrangerContext& ctx) {
        int beatsPerBackground = TimeData::instance().timeSignature.getNumerator() * 2;
        int firstBackgroundBeat = (ctx.firstVisibleBeat / beatsPerBackground) * beatsPerBackground;
        int backgroundIndex = firstBackgroundBeat / beatsPerBackground;
        bg_tone = (backgroundIndex % 2) == 0;
        for (int beat = firstBackgroundBeat; beat <= ctx.lastVisibleBeat; beat += beatsPerBackground) {
            DrawBackground(ctx, beat);
            bg_tone = !bg_tone;
        }
    }

    void DrawBackground(const ArrangerContext& ctx, int startBeat) {
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

    void DrawBarLine(const ArrangerContext& ctx, ImVec2 start, ImVec2 end, bool barStart) {
        ctx.drawList->AddLine(
            start, end,
            barStart ? Theme::currentThemeColours.barColourPacked
                     : Theme::currentThemeColours.beatColourPacked,
            1.0f
        );
    }


    void DrawTrackSeparator(const ArrangerContext& ctx) {
        for (unsigned int i = 0; i < ArrangerManager::instance().getTrackAmount(); i++) {
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