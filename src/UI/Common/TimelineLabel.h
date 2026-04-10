#pragma once
#include "imgui.h"
#include "../../Singletons/ViewState.h"
#include "../../Singletons/TimeData.h"
#include "../../Theme.h"

class TimelineLabel {
public:
    int totalBars = 30;
    float barWidth;
    float m_zoomFactor;
    float height = 20.0f;
    bool mouseDown = false;
    bool firstMouseUp = false;
    bool m_isArranger = false;
    TimelineLabel() = default;

    void create(float timelineLength, float &xScroll, int bars, bool isArranger);
    void DrawBarLabel();
    void DrawPlayHead();
    void HandleMouse();
};