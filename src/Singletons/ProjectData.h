#pragma once
#include <vector>

class ProjectData {
public:
    static ProjectData& instance() {
        static ProjectData instance;
        return instance;
    }

    [[nodiscard]] int getTotalBars() const { return totalBars; }
    [[nodiscard]] int getTrackAmount() const { return TrackAmount; }
    [[nodiscard]] int getTotalBarsPianoRoll() const { return totalBarsPianoRoll; }
    
    void setTotalBars(float newTotalBars) {totalBars = newTotalBars;}
    void addTrack() {TrackAmount++;}
    void setTrackAmount(int newTrackAmount) {TrackAmount = newTrackAmount;}

private:

    ProjectData() = default;

    int TrackAmount = 0;
    int totalBars = 30;
    int totalBarsPianoRoll = 30;

};
