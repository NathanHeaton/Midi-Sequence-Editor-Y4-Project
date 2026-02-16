// MidiPlayer.h
#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include "AudioManager.h"
#include "../Singletons/SessionData.h"
#include  "../Singletons/PatternManager.h"

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
    void setPlayingPtr(bool* playingPTR) {
        m_playingPtr = playingPTR;
    }

    void setPlaying(bool playing);
    bool isPlaying() const;

    void playCurrentPattern() {
        pattern = &PatternManager::instance().getCurrentPattern();
        eventIndex = 0;
        startTime = juce::Time::getMillisecondCounterHiRes();
        startTimer(1);
    }

    void pauseCurrentPattern() {
        midiOutput->clearAllPendingMessages();
        offMessageForPlayingEvents();
    }

    void timerCallback() override {
        if (eventIndex >= pattern->m_events.size()) {
            setPlaying(false);
            stopTimer();
            currentPatternElapsed = 0;
            return;
        }
        double elapsed = (juce::Time::getMillisecondCounterHiRes() + currentPatternElapsed - (startTime));
        double msPerTick = (60000.0 / static_cast<double>(SessionData::instance().getBPM())) / SessionData::instance().getPPQ();

        auto& event = pattern->m_events[eventIndex];

        double eventInMs = msPerTick * static_cast<double>(event.getAbsoluteTime());

        if (elapsed >= eventInMs) {
            juce::MidiMessage note;
            if (event.isNoteOff()) {
                note = juce::MidiMessage::noteOff(event.getChannel(), event.getPitch());
                removePlayedEvents(event);
            } else {
                note = juce::MidiMessage::noteOn(event.getChannel(), event.getPitch(),
                                                 (juce::uint8)event.getVelocity());
                m_playingEvents.push_back(note);
            }

            //Send to internal synth
            audioManager.addMidiMessage(note);
            // Send to external MIDI device
            if (midiOutput != nullptr) {
                midiOutput->sendMessageNow(note);
            }
            eventIndex++;
        }
        if (!isPlaying()) {
            currentPatternElapsed += elapsed;
            stopTimer();
        }
    }

    void removePlayedEvents(auto& event) {
        for (auto i{0u};i < m_playingEvents.size(); ++i) {
            if (m_playingEvents.at(i).getChannel() == event.getChannel() &&
                m_playingEvents.at(i).getNoteNumber() == event.getPitch()) {
                m_playingEvents.erase(m_playingEvents.begin()+i);
                }
        }
    }

    void offMessageForPlayingEvents() {
        for (auto event : m_playingEvents) {
            juce::MidiMessage note = juce::MidiMessage::noteOff(event.getChannel(), event.getNoteNumber());
            //Send to internal synth
            audioManager.addMidiMessage(note);
            // Send to external MIDI device
            if (midiOutput != nullptr) {
                midiOutput->sendMessageNow(note);
            }
        }
        m_playingEvents.clear();
    }

private:
    std::vector<juce::MidiMessage> m_playingEvents;
    AudioManager& audioManager;
    std::unique_ptr<juce::MidiOutput> midiOutput;
    const Pattern* pattern = nullptr;
    size_t eventIndex = 0;
    double startTime = 0;
    double currentPatternElapsed = 0;
    bool* m_playingPtr{nullptr};
};