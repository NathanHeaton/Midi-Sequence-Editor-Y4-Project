//
// Created by nathan on 30/11/2025.
//

#ifndef MYPROJECT_SESSIONDATA_H
#define MYPROJECT_SESSIONDATA_H

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_data_structures/juce_data_structures.h>
#include "Project_Data/Pattern.h"
#include "MIDI_Logic/ParsedMidi.h"

namespace Division {
    const float WhOLE = 4.0f;
    const float QUARTER_NOTE = 1.0f;
    const float EIGHT_NOTE = 0.5;
}

namespace zoomFactor {
    float arranger = 1.0f;
    float pianoRoll = 2.2f;
}


class SessionData {
public:
    class TimeSignature {
    public:
        TimeSignature(int n, int d) : numerator(n), denominator(d) {}

        void change(int n, int d) {
            numerator = n;
            denominator = d;
        }

        int getNumerator() const { return numerator; }
        int getDenominator() const { return denominator; }

    private:
        int numerator;
        int denominator;
    };

    static SessionData& instance() {
        static SessionData instance;
        return instance;
    }

    // Getters
    [[nodiscard]] int getBPM() const { return BPM; }
    [[nodiscard]] int getPPQ() const { return PPQ; }

    [[nodiscard]] int getTrackAmount() const { return TrackAmount; }
    [[nodiscard]] float getTrackHeight() const { return TrackHeight; }
    [[nodiscard]] int getTotalBars() const { return totalBars; }
    [[nodiscard]] int getTotalBarsPianoRoll() const { return totalBarsPianoRoll; }

    [[nodiscard]] float getPixelPer(float division , float zoomFactor) const {
        return pixelPerQuarterNote * zoomFactor * division;
    }

    [[nodiscard]] float getPixelPerBeat(float zoomFactor) const {        return pixelPerQuarterNote*zoomFactor;   }

    [[nodiscard]] float getPixelPerBar(float zoomFactor) const {
        return pixelPerQuarterNote * zoomFactor * timeSignature.getNumerator();
    }

    [[nodiscard]] int getWhiteKeys() const { return WHITE_KEYS; }
    [[nodiscard]] int getBlackKeys() const { return BLACK_KEYS; }
    [[nodiscard]] float getBlackGap() const { return Black_Gap; }
    [[nodiscard]] ImVec2 getWhiteSize() const { return WHITE_SIZE; }
    [[nodiscard]] ImVec2 getBlackSize() const { return BLACK_SIZE; }

    // Setters
    void setBPM(int newBPM) {        BPM = newBPM;    }

    void setPianoRollZoomFactor(float newPianoRollZoomFactor) { pianoRollZoomFactor = newPianoRollZoomFactor; }
    void setArrangerZoomFactor(float newArranger) { arrangerZoomFactor = newArranger; }

    void setTrackHeight(float newTrackHeight) {TrackHeight = newTrackHeight;}
    void setTotalBars(float newTotalBars) {totalBars = newTotalBars;}
    void setTotalBarsPianoRoll(float newTotalBarsPianoRoll) {totalBarsPianoRoll = newTotalBarsPianoRoll;}
    void setBarWidth(float newBarWidth) {barWidth = newBarWidth;}
    void setBarWidthPianoRoll(float newBarWidthPianoRoll) {barWidthPianoRoll = newBarWidthPianoRoll;}
    void addTrack() {TrackAmount++;}
    void setTrackAmount(int newTrackAmount) {TrackAmount = newTrackAmount;}

    void addPattern() {
        std::string title = "untitled " + std::to_string(pattern.size());
        pattern.emplace_back(Pattern(title));
    }
    void addPatternFromMidi(std::string title, auto& events) {
        pattern.emplace_back(Pattern(title,events));
    }
    bool anyPatterns() {
        return !pattern.empty();
    }
    const std::vector<Pattern>& getPatterns() {
        return pattern;
    }
    const Pattern& getCurrentPattern() {
        return pattern.at(activePattern);
    }

    void setCurrentPattern(size_t newPattern) {activePattern = newPattern;}

    void addParsedMidi(auto& data, std::string title) {
        parsedMidiFile.emplace_back(data,title);
        updatePatternWithMidiData();
    }

    void updatePatternWithMidiData() {

        auto& currentFile = parsedMidiFile.at(parsedMidiFile.size()-1);
        size_t tracks = currentFile.m_tracks.size();
        for (size_t track = 0; track < tracks; track++) {
            auto title =  currentFile.m_title + " " + std::to_string(track);
            addPatternFromMidi(title,currentFile.m_tracks.at(track).Events, currentFile.ticksInQuarterNote;
        }
        setCurrentPattern(pattern.size()-1);

    }


    size_t getPatternSize() {return pattern.size();}

    // void addPatternFromMidi(std::string title,std::vector<> events) {
    //     pattern.emplace_back(Pattern(title));
    // }

    TimeSignature timeSignature{4, 4};

private:
    JUCE_DECLARE_NON_COPYABLE(SessionData)
    SessionData() = default;

    int PPQ = 960;

    int BPM = 120;
    int TrackAmount = 0;

    const float pixelPerQuarterNote = 32.0f;

    float pianoRollZoomFactor = 1.0f;
    float arrangerZoomFactor = 1.0f;

    float pixelPerBeat = 30.0f;
    float TrackHeight = 100.0f;

    int WHITE_KEYS{75};
    int BLACK_KEYS{53};

    ImVec2 WHITE_SIZE{100, 24};
    ImVec2 BLACK_SIZE{80, 15};

    float barWidth = timeSignature.getNumerator() * pixelPerBeat;
    float barWidthPianoRoll ;//= timeSignature.getNumerator() * pixelPerBeatPianoRoll;

    float Black_Gap{WHITE_SIZE.y - BLACK_SIZE.y / 2};

    int totalBars = 30;
    int totalBarsPianoRoll = 30;

    std::vector<Pattern> pattern{};
    size_t activePattern = 0;

    std::vector<ParsedMidi> parsedMidiFile;


};

#endif //MYPROJECT_SESSIONDATA_H