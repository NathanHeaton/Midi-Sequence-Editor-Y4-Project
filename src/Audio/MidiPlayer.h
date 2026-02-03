// MidiPlayer.h
#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include "../Singletons/SessionData.h"

class MidiPlayer : public juce::Timer {
public:
    MidiPlayer(AudioManager& audioMgr) : audioManager(audioMgr) {
        auto devices = juce::MidiOutput::getAvailableDevices();
        for (auto& device : devices) {
            DBG("devices " << device.name << " " << device.identifier);
        }
        if (!devices.isEmpty()) {
            midiOutput = juce::MidiOutput::openDevice(devices[0].identifier);
        }
    }

    void playCurrentPattern() {
        pattern = &SessionData::instance().getCurrentPattern();
        eventIndex = 0;
        startTime = juce::Time::getMillisecondCounterHiRes();
        startTimer(1);
    }

    void timerCallback() override {
        if (eventIndex >= pattern->m_events.size()) {
            stopTimer();
            return;
        }

        double elapsed = (juce::Time::getMillisecondCounterHiRes() - startTime);
        double secondsPerTick = 60.0 / (SessionData::instance().getBPM() *
                                        SessionData::instance().getPPQ());

        auto& event = pattern->m_events[eventIndex];
        double eventTime = static_cast<double>(event.getAbsoluteTime()) *
                          secondsPerTick * 1000;

        if (elapsed >= eventTime) {
            DBG("Playing note at " << elapsed << "s, pitch: " << event.getPitch());

            juce::MidiMessage note;
            if (event.isNoteOff()) {
                note = juce::MidiMessage::noteOff(1, event.getPitch());
            } else {
                note = juce::MidiMessage::noteOn(1, event.getPitch(),
                                                 (juce::uint8)event.getVelocity());
            }

            // Send to internal synth
            audioManager.addMidiMessage(note);

            // Send to external MIDI device
            if (midiOutput != nullptr) {
                midiOutput->sendMessageNow(note);
            }

            eventIndex++;
        }
    }

private:
    AudioManager& audioManager;
    std::unique_ptr<juce::MidiOutput> midiOutput;
    const Pattern* pattern = nullptr;
    size_t eventIndex = 0;
    double startTime = 0;
};