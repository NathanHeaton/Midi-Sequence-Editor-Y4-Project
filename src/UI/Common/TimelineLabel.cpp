#include "TimelineLabel.h"
#include "../../Singletons/PlayBackManager.h"
#include "../../Singletons/ViewState.h"

void TimelineLabel::create(float timelineLength, float &xScroll, int bars, bool isArranger) {
    totalBars = bars + 1;
    m_isArranger = isArranger;
    if (isArranger) {m_zoomFactor = zoomFactor::arranger; }
    else {m_zoomFactor = zoomFactor::pianoRoll;}
    barWidth = ViewState::instance().getPixelPerBar(m_zoomFactor);

    if (ImGui::BeginChild("Timeline", ImVec2(0, height), false)) {
        ImGui::SetScrollX(xScroll);
        ImGui::Dummy(ImVec2(timelineLength, 0));
        DrawBarLabel();
        DrawPlayHead();
        HandleMouse();
    } ImGui::EndChild();
}

void TimelineLabel::DrawBarLabel() {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();
    for (int bar = 0; bar < totalBars; bar++) {
        char label[16];
        snprintf(label, sizeof(label), "%d", bar);
        drawList->AddText(
            ImVec2((bar * barWidth) + cursorPos.x + 2, cursorPos.y),
            Theme::cTheme.barColourPacked,
            label
        );
        drawList->AddLine(
            ImVec2(cursorPos.x + bar * barWidth, cursorPos.y),
            ImVec2(cursorPos.x + bar * barWidth, cursorPos.y + 20),
            Theme::cTheme.barColourPacked, 1
        );
    }
}

void TimelineLabel::DrawPlayHead() {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto drawList = ImGui::GetWindowDrawList();

    double ticks =  (m_isArranger) ?
    PlayBackManager::instance().getArrangerPlayheadTicks() :
    PlayBackManager::instance().getPianoRollPlayheadTicks();

    float xPos = cursorPos.x + ViewState::instance().getPixelPerBeat(m_zoomFactor) * (ticks / TimeData::instance().PPQ);

    drawList->AddLine(
        ImVec2(xPos, cursorPos.y),
        ImVec2(xPos, cursorPos.y + height),
        Theme::cTheme.accentPacked, 3
    );
}

void TimelineLabel::HandleMouse() {
    auto cursorPos = ImGui::GetCursorScreenPos();
    auto mousePos = ImGui::GetMousePos();

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
        PlayBackManager::instance().stop();
        unsigned int ticks = TimeData::instance().PPQ *
            ((mousePos.x - cursorPos.x) / ViewState::instance().getPixelPerBeat(m_zoomFactor));
        PlayBackManager::instance().seekToTicks(ticks, m_isArranger);
        mouseDown = true;
    }
    else if (mouseDown) { firstMouseUp = true; mouseDown = false; }

    if (firstMouseUp) {
        firstMouseUp = false;
    }
}