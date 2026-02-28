#pragma once

#include "../../../Theme.h"
#include <imgui.h>
#include <iostream>
#include "../../../Singletons/ViewState.h"

class Piano
{
public:
    
    Piano() = default;
    
    struct pianoVars {
        ImVec2 cursorPos;
        ImDrawList* drawList;
        float height;
        float scrollY;

        pianoVars() {
            cursorPos = ImGui::GetCursorScreenPos();
            drawList = ImGui::GetWindowDrawList();
            height = ImGui::GetWindowHeight();
            scrollY = ImGui::GetScrollX();
        }
    };
    void create(const float &pianoRollScrollY) {
        whiteKeys = ViewState::instance().WHITE_KEYS;
        blackKeys = ViewState::instance().BLACK_KEYS;
        WHITE_SIZE = ViewState::instance().getWhiteSize();
        BLACK_SIZE = ViewState::instance().getBlackSize();
        blackGap = ViewState::instance().getBlackGap();
        if ( ImGui::BeginChild("piano",ImVec2(WHITE_SIZE.x,0),ImGuiChildFlags_None,
            ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse)) {

            pianoVars vars;
            drawWhiteKey(vars);

            ImGui::SetScrollY(pianoRollScrollY);

            drawBlackKey(vars);
            ImGui::Dummy(ImVec2(WHITE_SIZE.x,WHITE_SIZE.y*whiteKeys));
        }
        ImGui::EndChild();
    }

    void drawWhiteKey(pianoVars &vars) const
    {
        int octave = 9;
        for ( int i{0u}; i < whiteKeys; ++i ) {
            ImVec2 rectStart = ImVec2(vars.cursorPos.x , vars.cursorPos.y + i* WHITE_SIZE.y);

            ImVec2 rectEnd = ImVec2(vars.cursorPos.x + WHITE_SIZE.x , vars.cursorPos.y + (1 + i)* WHITE_SIZE.y -1);

            ImVec2 textPos = ImVec2(rectStart.x + WHITE_SIZE.x -20,rectStart.y);
            vars.drawList->AddRectFilled(
                rectStart, rectEnd,
                Theme::currentThemeColours.barColourPacked,
                6.0f
            );
            if ((i - 4) % 7 == 0 && i >= 4) {
                char octaveStr[3];
                octaveStr[0] = 'c';
                octaveStr[1] = '0' + octave;
                octaveStr[2] = '\0';
                octave--;
                vars.drawList->AddText(
                    textPos,
                    Theme::currentThemeColours.backgroundPacked,
                    octaveStr
                );
            }
        }
    }
    void drawBlackKey(pianoVars &vars) const
    {
        float totalGap = 0;
        int setOf3Count = 3;
        int setOf2Count = 1;
        for ( int i{0u}; i < blackKeys; ++i ) {
            if (i == 0) {
                totalGap = totalGap + blackGap;
            }
            else {
                totalGap += WHITE_SIZE.y;
            }

            ImVec2 rectStart = ImVec2(vars.cursorPos.x , vars.cursorPos.y + totalGap);
            ImVec2 rectEnd = ImVec2(vars.cursorPos.x + BLACK_SIZE.x , vars.cursorPos.y + totalGap + BLACK_SIZE.y );

            vars.drawList->AddRectFilled(
            rectStart, rectEnd,
            Theme::currentThemeColours.backgroundPacked,
            6.0f
        );
            if (setOf3Count < 3) {
                setOf3Count++;
            }
            else if (setOf3Count == 3) {
                setOf3Count++;
                totalGap += WHITE_SIZE.y;
            }
            else if (setOf2Count < 2) {
                setOf2Count++;
            }
            else if (setOf2Count == 2) {
                setOf3Count = 1;
                setOf2Count = 1;
                totalGap += WHITE_SIZE.y;
            }
        }
    }

private:
    int whiteKeys {};
    int blackKeys {};
    ImVec2 WHITE_SIZE {};
    ImVec2 BLACK_SIZE {};
    float blackGap {};
};
