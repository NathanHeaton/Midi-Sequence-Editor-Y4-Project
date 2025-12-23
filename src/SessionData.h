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
    class Listener {
    public:
        virtual ~Listener() =default;
        virtual void bpmChanged(int /*newBPM*/) {};
        virtual void denominatorChanged(int /*newDenominator*/) {};
        virtual void numeratorChanged(int /*newNumerator*/) {};
        virtual void barWidthChanged(float /*newBarWidth*/) {};
    };

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

    void setPixelPerBeat(float newPixelPerBeat) {pixelPerBeat = newPixelPerBeat;}
    void setBPM(int newBPM) {
        BPM = newBPM;
        listeners.call(&Listener::bpmChanged, newBPM);
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
    juce::ListenerList<Listener> listeners;


    private:

    JUCE_DECLARE_NON_COPYABLE(SessionData)
    SessionData() = default;
    int BPM =120;
    int TrackAmount = 0;
    float pixelPerBeat = 30.0f;
    float barWidth = 200.0f;
    float TrackHeight = 100.0f;





};
#endif //MYPROJECT_SESSIONDATA_H