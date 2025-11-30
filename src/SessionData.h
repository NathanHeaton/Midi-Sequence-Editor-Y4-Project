//
// Created by nathan on 30/11/2025.
//

#ifndef MYPROJECT_SESSIONDATA_H
#define MYPROJECT_SESSIONDATA_H


#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_data_structures/juce_data_structures.h>

#include "UI/Piano_Roll/Panels/PianoRoll/PianoKeys.h"

class SessionData {
    public:
    static SessionData& instance() {
        static SessionData instance;
        return instance;
    }

    int getBPM() const {return BPM;}
    int getNumerator() const {return numerator;}
    int getDenominator() const {return denominator;}
    float getBarWidth() const {return barWidth;}

    void setBPM(int newBPM) {
        BPM = newBPM;
        listeners.call(&Listener::bpmChanged, newBPM);
    }

    void setNumerator(int newNumerator) {
        numerator = newNumerator;
        listeners.call(&Listener::numeratorChanged, newNumerator);
    }

    void setDenominator(int newDenominator) {
        denominator = newDenominator;
        listeners.call(&Listener::denominatorChanged, newDenominator);
    }

    void setBarWidth(float newBarWidth) {
        barWidth = newBarWidth;

    }

    class Listener {
        public:
        virtual ~Listener() =default;
        virtual void bpmChanged(int newBPM)=0;
        virtual void denominatorChanged(int newDenominator)=0;
        virtual void numeratorChanged(int newNumerator)=0;
        virtual void barWidthChanged(int newBarWidth)=0;
    };

    juce::ListenerList<Listener> listeners;

    private:
    SessionData();
    JUCE_DECLARE_NON_COPYABLE(SessionData);

    int BPM =120;
    int numerator =4;
    float barWidth = 200.0f;
    int denominator =4;



};
#endif //MYPROJECT_SESSIONDATA_H