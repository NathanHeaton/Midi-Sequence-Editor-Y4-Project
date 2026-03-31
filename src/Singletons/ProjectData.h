#pragma once
#include <vector>

class ProjectData {
public:
    static ProjectData& instance() {
        static ProjectData instance;
        return instance;
    }

    [[nodiscard]] int getTotalBars() const { return totalBars; }

    [[nodiscard]] int getTotalBarsPianoRoll() const { return totalBarsPianoRoll; }
    
    void setTotalBars(float newTotalBars) {totalBars = newTotalBars;}


private:

    ProjectData() = default;
    int totalBars = 30;
    int totalBarsPianoRoll = 30;

};
