//
// Created by nathan on 18/01/2026.
//
#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "../Singletons/SessionData.h"

class MidiPlayer : public juce::AudioSource, juce::Timer{
public:
    MidiPlayer() {
        audioDeviceManager.initialiseWithDefaultDevices(0,2);
        //audioDeviceManager.addAudioCallback(this);
    }

    ~MidiPlayer() {
        //audioDeviceManager.removeAudioCallback(this);
    }

    //void loadMidi(const )

    void playCurrentPattern() {
        pattern = &SessionData::instance().getCurrentPattern();
        startTime = juce::Time::getMillisecondCounterHiRes();
        startTimer(1);
    }

    void timerCallback() override {
        if (eventIndex>= pattern->m_events.size()) {
            stopTimer();
            return;
        }

        double elapsed = (juce::Time::getMillisecondCounterHiRes() - startTime);
        double secondsPerTick = 60.0 /(SessionData::instance().getBPM() * SessionData::instance().getPPQ());

        auto& event = pattern->m_events[eventIndex];
        double eventTime = event.getAbsoluteTime();

        if (elapsed >= eventTime) {
            DBG("Playing note at"<< elapsed<< "s"<<" Note pitch: "<< event.getPitch());
            eventIndex++;
        }
    }



private:
    juce::AudioDeviceManager audioDeviceManager;
    juce::MidiMessageSequence midiMessageSequence;

    const Pattern* pattern = nullptr;
    int bpm = 120;
    size_t eventIndex = 0;
    double startTime = 0;
};

