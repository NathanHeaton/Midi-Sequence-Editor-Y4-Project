#pragma once
#include <vector>

class ProjectData {
public:
    static ProjectData& instance() {
        static ProjectData instance;
        return instance;
    }

    [[nodiscard]] unsigned int getTotalBars() const { return totalBars; }

    [[nodiscard]]  unsigned int getTotalBarsPianoRoll() const { return totalBarsPianoRoll; }
    
    void setTotalBars(float newTotalBars) {totalBars = newTotalBars;}

    float masterVolume = 1.0f;
private:

    ProjectData() = default;

     unsigned int totalBars = 30;
     unsigned int totalBarsPianoRoll = 30;


};
