//
// Created by nathan on 18/01/2026.
//
#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "../Singletons/SessionData.h"


class MidiPlayer : public juce::Timer, public juce::AudioSource{
public:

    MidiPlayer() {
        auto deviceIndex = juce::MidiOutput::getAvailableDevices();
        for (auto& device : deviceIndex) {
            DBG("devices "<<device.name << device.identifier);
        }
        midiOutput = juce::MidiOutput::openDevice(deviceIndex[0].identifier);
    }

    ~MidiPlayer() {
        //audioDeviceManager.removeAudioCallback(this);
    }

    //void loadMidi(const )

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        tempSynth.setCurrentPlaybackSampleRate(sampleRate);

    }

    void playCurrentPattern() {
        pattern = &SessionData::instance().getCurrentPattern();
        startTime = juce::Time::getMillisecondCounterHiRes();
        startTimer(1);
    }

    void timerCallback() override {
        if (eventIndex >= pattern->m_events.size()) {
            stopTimer();
            return;
        }

        double elapsed = (juce::Time::getMillisecondCounterHiRes() - startTime);
        double secondsPerTick = 60.0 /(SessionData::instance().getBPM() * SessionData::instance().getPPQ());


        auto& event = pattern->m_events[eventIndex];
        double eventTime = static_cast<double>(event.getAbsoluteTime()) * secondsPerTick * 1000;

        DBG("time in milliseconds of next event: "<<eventTime<<" elapsed Time: "<<elapsed);
        if (elapsed >= eventTime) {
            DBG("Playing note at"<< elapsed<< "s"<<" Note pitch: "<< event.getPitch());
            auto note = juce::MidiMessage::noteOn(0, event.getPitch(), event.getVelocity());
            if (event.isNoteOff()) {
                note = juce::MidiMessage::noteOff(0, event.getPitch(), event.getVelocity());
            }

            midiOutput->sendMessageNow(note);

            eventIndex++;
        }
    }



private:
    juce::Synthesiser tempSynth;
    std::unique_ptr<juce::MidiOutput> midiOutput;
    const Pattern* pattern = nullptr;
    int bpm = 120;
    size_t eventIndex = 0;
    double startTime = 0;
};

