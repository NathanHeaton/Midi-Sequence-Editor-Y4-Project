#pragma once
#include <vector>

class ProjectData {
public:
    static ProjectData& instance() {
        static ProjectData instance;
        return instance;
    }

    [[nodiscard]] uint getTotalBars() const { return totalBars; }

    [[nodiscard]] uint getTotalBarsPianoRoll() const { return totalBarsPianoRoll; }
    
    void setTotalBars(float newTotalBars) {totalBars = newTotalBars;}


private:

    ProjectData() = default;
    uint totalBars = 30;
    uint totalBarsPianoRoll = 30;


};
