#pragma once
#include <vector>
#include "imgui.h"

struct PatternPanelState {
    size_t      patternID;
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

    void openPatern(size_t ID = 0) {
        for (auto& p : m_paternPanels)
            if (p.patternID == ID) { p.open = true; return; }

        newPaternPanelJustCreated = true;
        m_paternPanels.push_back({ ID });
    }

    void closePanel(size_t index)  { m_paternPanels.at(index).open = false; }
    std::deque<PatternPanelState>& getPanels() { return m_paternPanels; }


private:
    PanelManager() = default;
    std::deque<PatternPanelState> m_paternPanels;
};