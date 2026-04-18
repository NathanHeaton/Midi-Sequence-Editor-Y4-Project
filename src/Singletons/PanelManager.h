#pragma once
#include <vector>
#include <deque>
#include "imgui.h"

struct PatternPanelState {
    size_t      patternID;
    size_t      trackIndex;
    bool        open  = true;
    ImVec2      pos   = {100, 100};
    ImVec2      size  = {900, 500};
};

class PanelManager {
public:
    bool newPaternPanelJustCreated = false;

    static PanelManager& instance() {
        static PanelManager s;
        return s;
    }

    void openPatern(size_t ID = 0, size_t trackIndex = 0) {
        for (auto& p : m_paternPanels)
            if (p.patternID == ID) { p.open = true; return; }

        newPaternPanelJustCreated = true;
        ImVec2 pos = {300,300};
        if (!m_paternPanels.empty()) {
            pos = {m_paternPanels.back().pos.x +OFFSET.x, m_paternPanels.back().pos.y + OFFSET.y};
        }

        m_paternPanels.push_back({ ID, trackIndex });
        m_paternPanels.back().pos = pos;
    }

    void closePanel(size_t index)  { m_paternPanels.at(index).open = false; }
    std::deque<PatternPanelState>& getPanels() { return m_paternPanels; }


private:
    ImVec2 OFFSET = {50.0f, 50.0f};
    PanelManager() = default;
    std::deque<PatternPanelState> m_paternPanels;
};