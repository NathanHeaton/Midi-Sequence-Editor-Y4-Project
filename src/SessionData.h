//
// Created by nathan on 30/11/2025.
//

#ifndef MYPROJECT_SESSIONDATA_H
#define MYPROJECT_SESSIONDATA_H

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_data_structures/juce_data_structures.h>
#include "Project_Data/Pattern.h"


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
    int getBPM() const { return BPM; }
    float getBarWidth() const { return barWidth; }
    int getTrackAmount() const { return TrackAmount; }
    float getPixelPerBeat() const { return pixelPerBeat; }
    float getTrackHeight() const { return TrackHeight; }
    float getTotalBars() const { return totalBars; }
    float getTotalBarsPianoRoll() const { return totalBarsPianoRoll; }
    float getBarWidthPianoRoll() const { return barWidthPianoRoll; }
    float getPixelPerBeatPianoRoll() const { return pixelPerBeatPianoRoll; }

    int getWhiteKeys() const { return WHITE_KEYS; }
    int getBlackKeys() const { return BLACK_KEYS; }
    float getBlackGap() const { return Black_Gap; }
    ImVec2 getWhiteSize() const { return WHITE_SIZE; }
    ImVec2 getBlackSize() const { return BLACK_SIZE; }

    // Setters
    void setBPM(int newBPM) {        BPM = newBPM;    }

    void setPixelPerBeat(float newPixelPerBeat) {
        pixelPerBeat = newPixelPerBeat;
        barWidth = timeSignature.getNumerator() * pixelPerBeat;
    }

    void setPixelPerBeatPianoRoll(float newPixelPerBeatPianoRoll) {
        pixelPerBeatPianoRoll = newPixelPerBeatPianoRoll;
        barWidthPianoRoll = timeSignature.getNumerator() * pixelPerBeatPianoRoll;
    }

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

    bool anyPatterns() {
        return !pattern.empty();
    }

    const std::vector<Pattern>& getPatterns() {
        return pattern;
    }

    const Pattern& getCurrentPattern() {
        return pattern.at(activePattern);
    }

    size_t getPatternSize() {return pattern.size();}

    // void addPatternFromMidi(std::string title,std::vector<> events) {
    //     pattern.emplace_back(Pattern(title));
    // }

    TimeSignature timeSignature{4, 4};

private:
    JUCE_DECLARE_NON_COPYABLE(SessionData)
    SessionData() = default;

    int BPM = 120;
    int TrackAmount = 0;
    float pixelPerBeat = 30.0f;
    float pixelPerBeatPianoRoll = 45.0f;
    float TrackHeight = 100.0f;

    int WHITE_KEYS{52};
    int BLACK_KEYS{36};

    ImVec2 WHITE_SIZE{100, 24};
    ImVec2 BLACK_SIZE{80, 15};

    float barWidth = timeSignature.getNumerator() * pixelPerBeat;
    float barWidthPianoRoll = timeSignature.getNumerator() * pixelPerBeatPianoRoll;

    float Black_Gap{WHITE_SIZE.y - BLACK_SIZE.y / 2};

    float totalBars = 30;
    float totalBarsPianoRoll = 30;

    std::vector<Pattern> pattern{};
    size_t activePattern = 0;

};

#endif //MYPROJECT_SESSIONDATA_H