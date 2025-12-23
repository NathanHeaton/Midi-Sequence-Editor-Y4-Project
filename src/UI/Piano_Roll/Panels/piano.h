#pragma once


#include "../../../Theme.h"
#include <imgui.h>
#include <iostream>


class Piano
{
public:
    const int WHITE_KEYS{52};
    const int BLACK_KEYS{36};

    const ImVec2 WHITE_SIZE{100,24};
    const float Black_Gap{10};
    const ImVec2 BLACK_SIZE{80,15};

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



    void create() {
        if ( ImGui::BeginChild("piano",ImVec2(WHITE_SIZE.x,400),false,ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
            pianoVars vars;
            ImGui::Text("200049");
            drawWhiteKey(vars);
            drawBlackKey(vars);
            ImGui::Dummy(ImVec2(WHITE_SIZE.x,WHITE_SIZE.y*WHITE_KEYS));
        }
        ImGui::EndChild();

    }

    void drawWhiteKey(pianoVars &vars) {
        for ( int i{0u}; i < WHITE_KEYS; ++i ) {
            ImVec2 rectStart = ImVec2(vars.cursorPos.x , vars.cursorPos.y + i* WHITE_SIZE.y);
            ImVec2 rectEnd = ImVec2(vars.cursorPos.x + WHITE_SIZE.x , vars.cursorPos.y + (1 + i)* WHITE_SIZE.y -1);

            vars.drawList->AddRectFilled(
            rectStart, rectEnd,
            Theme::currentThemeColours.barColourPacked,
            6.0f
        );

        }
    }

    void drawBlackKey(pianoVars &vars) {
        for ( int i{0u}; i < BLACK_KEYS; ++i ) {

            ImVec2 rectStart = ImVec2(vars.cursorPos.x , vars.cursorPos.y + i* BLACK_SIZE.y + Black_Gap);
            ImVec2 rectEnd = ImVec2(vars.cursorPos.x + BLACK_SIZE.x , vars.cursorPos.y + (1 + i)* BLACK_SIZE.y + Black_Gap);

            vars.drawList->AddRectFilled(
            rectStart, rectEnd,
            Theme::currentThemeColours.backgroundPacked,
            6.0f
        );

        }
    }

private:

};
