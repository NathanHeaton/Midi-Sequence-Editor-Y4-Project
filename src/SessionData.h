//
// Created by nathan on 30/11/2025.
//

#ifndef MYPROJECT_SESSIONDATA_H
#define MYPROJECT_SESSIONDATA_H


#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_data_structures/juce_data_structures.h>

class SessionData {
    public:

    class TimeSignature {
    public:
        int numerator;
        int denominator;
        TimeSignature(int n, int d): numerator(n), denominator(d) {}
        void change(int n,int d){numerator=n;denominator=d;}
        int getNumerator() const {return numerator;}
        int getDenominator() const {return denominator;}
    };

    static SessionData& instance() {
        static SessionData instance;
        return instance;
    }

    int getBPM() const {return BPM;}
    float getBarWidth() const {return barWidth;}
    int getTrackAmount() const {return TrackAmount;}
    float getPixelPerBeat() const {return pixelPerBeat;}
    float getTrackHeight() const {return TrackHeight;}

    int getWhiteKeys() const {return WHITE_KEYS;}
    int getBlackKeys() const {return BLACK_KEYS;}
    float getBlackGap() const {return Black_Gap;}
    ImVec2 getWhiteSize() const {return WHITE_SIZE;}
    ImVec2 getBlackSize() const {return BLACK_SIZE;}

    void setPixelPerBeat(float newPixelPerBeat) {pixelPerBeat = newPixelPerBeat;}
    void setBPM(int newBPM) {
        BPM = newBPM;

    }
    void setBarWidth(float newBarWidth) {
        barWidth = newBarWidth;
    }

    void addTrack() {
        TrackAmount++;
    }

    void setTrackHeight(float newTrackHeight) {
        TrackHeight = newTrackHeight;
    }

    TimeSignature timeSignature{4,4};

    private:

    JUCE_DECLARE_NON_COPYABLE(SessionData)
    SessionData() = default;
    int BPM =120;
    int TrackAmount = 0;
    float pixelPerBeat = 30.0f;
    float barWidth = 200.0f;
    float TrackHeight = 100.0f;

    int WHITE_KEYS{52};
    int BLACK_KEYS{36};

    ImVec2 WHITE_SIZE{100,24};
    ImVec2 BLACK_SIZE{80,15};

    float Black_Gap{WHITE_SIZE.y-BLACK_SIZE.y/2};




};
#endif //MYPROJECT_SESSIONDATA_H