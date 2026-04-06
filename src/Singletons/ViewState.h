#pragma once
#include <vector>

#include "imgui.h"
#include "TimeData.h"

inline namespace zoomFactor {
    inline float arranger = 1.0f;
    inline float pianoRoll = 3.2f;
}
inline namespace Division {
    const float WHOLE = 4.0f;
    const float QUARTER_NOTE = 1.0f;
    const float EIGHT_NOTE = 0.5;
}
class ViewState {
public:
    static ViewState& instance() {
        static ViewState instance;
        return instance;
    }

    [[nodiscard]] float getPixelPer(float division , float zoomFactor) const {
        return pixelPerQuarterNote * zoomFactor * division;
    }
    [[nodiscard]] float getPixelPerBeat(float zoomFactor) const {        return pixelPerQuarterNote*zoomFactor;   }

    [[nodiscard]] float getPixelPerBar(float zoomFactor) const {
        return pixelPerQuarterNote * zoomFactor * TimeData::instance().timeSignature.getNumerator();
    }

    [[nodiscard]] char getRenderedSubDivisions() const { return subdivisionsPianoRoll.renderList[subdivisionsPianoRoll.renderedIndex]; }
    [[nodiscard]] long getSnappedSubDivisions() const { return subdivisionsPianoRoll.snapList[subdivisionsPianoRoll.snapIndex]; }

    [[nodiscard]] char getRenderedSubDivisionsArr() const { return subdivisionsArr.renderList[subdivisionsPianoRoll.renderedIndex]; }
    [[nodiscard]] long getSnappedSubDivisionsArr() const { return subdivisionsArr.snapList[subdivisionsPianoRoll.snapIndex]; }

    [[nodiscard]] float getTrackHeight() const { return TrackHeight; }
    [[nodiscard]] uint32_t getStandardSnapTime() const
    {
        if (subdivisionsPianoRoll.snapList.end() == subdivisionsPianoRoll.snapList.begin()+subdivisionsPianoRoll.snapIndex)
        {
            return static_cast<float>(TimeData::PPQ) / subdivisionsPianoRoll.snapList.at(3);
        }
        return static_cast<float>(TimeData::PPQ)  / subdivisionsPianoRoll.snapList.at(subdivisionsPianoRoll.snapIndex);
    }

    void setPianoRollZoomFactor(float newPianoRollZoomFactor) { pianoRollZoomFactor = newPianoRollZoomFactor; }
    void setArrangerZoomFactor(float newArranger) { arrangerZoomFactor = newArranger; }
    void setTrackHeight(float newTrackHeight) {TrackHeight = newTrackHeight;}
    void setBarWidth(float newBarWidth) {barWidth = newBarWidth;}

    void setRenderedSubDivisions(int newDivisions) {subdivisionsPianoRoll.renderedIndex = newDivisions;}
    void setSnapSubDivisions(int newSnap) {subdivisionsPianoRoll.snapIndex = newSnap;}


    void setRenderedSubDivisionsArr(int newDivisions) {subdivisionsArr.renderedIndex = newDivisions;}
    void setSnapSubDivisionsArr(int newSnap) {subdivisionsArr.snapIndex = newSnap;}

    static constexpr int WHITE_KEYS{75};
    static constexpr int BLACK_KEYS{53};

    [[nodiscard]] float getBlackGap() const { return Black_Gap; }
    [[nodiscard]] ImVec2 getWhiteSize() const { return WHITE_SIZE; }
    [[nodiscard]] ImVec2 getBlackSize() const { return BLACK_SIZE; }
    [[nodiscard]] float getNoteHeight() const { return noteHeight; }

private:

    ViewState() = default;

    static constexpr float pixelPerQuarterNote = 32.0f;

    float pianoRollZoomFactor = 1.0f;
    float arrangerZoomFactor = 1.0f;

    float pixelPerBeat = 32.0f;
    float TrackHeight = 100.0f;
    float barWidth = TimeData::instance().timeSignature.getNumerator() * pixelPerBeat;

    ImVec2 WHITE_SIZE{100, 32};
    ImVec2 BLACK_SIZE{WHITE_SIZE.x * 0.8f, WHITE_SIZE.y * 0.625f};

    float noteHeight = WHITE_SIZE.y * 7 /12;
    float Black_Gap{WHITE_SIZE.y - BLACK_SIZE.y / 2};

    struct Subdivision
    {
        std::vector<char> renderList = {1,2,3,4,5,6,8};
        std::vector<long> snapList = {1,2,3,4,5,6,8,12,16,24,32, TimeData::instance().PPQ};

        size_t renderedIndex =3;
        size_t snapIndex=3;

        Subdivision(size_t defaultInex) {
            renderedIndex = defaultInex;
            snapIndex = defaultInex;
        }
    };

    Subdivision subdivisionsArr{3};
    Subdivision subdivisionsPianoRoll{0};
};

