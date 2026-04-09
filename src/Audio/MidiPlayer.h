#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include "AudioManager.h"
#include "EventComplier.h"
#include "../Singletons/TimeData.h"

enum class PlaybackSource { PianoRoll, Arranger };

class MidiPlayer : public juce::Timer {
public:
    explicit MidiPlayer(AudioManager& audio) : m_audio(audio) {}
    ~MidiPlayer() { cleanUp(); }

    void cleanUp() {
        stopTimer();
        allNotesOff();
        m_midiOut.reset();
    }

    void initialiseMidiPlayer() {
        auto devices = juce::MidiOutput::getAvailableDevices();
        if (!devices.isEmpty())
            m_midiOut = juce::MidiOutput::openDevice(devices[0].identifier);
    }

    void setPlayingPtr(bool* ptr) { m_playingPtr = ptr; }

    void play(PlaybackSource source) {
        const double bpm = TimeData::instance().getBPM();

        if (source == PlaybackSource::PianoRoll) {
            m_events = EventCompiler::compilePattern(
                PatternManager::instance().getCurrentPattern(), bpm);
        } else {
            m_events = EventCompiler::compileArranger(bpm);
        }

        // Find the first event at or after the current playhead position
        m_eventIndex = firstEventAtOrAfter(m_playheadMs);
        m_wallClockStart = juce::Time::getMillisecondCounterHiRes() - m_playheadMs;
        startTimer(1);
    }

    void pause() {
        stopTimer();
        allNotesOff();
        // m_playheadMs already holds the current position — nothing extra needed
    }

    void stop() {
        stopTimer();
        allNotesOff();
        m_playheadMs = 0.0;
        m_eventIndex = 0;
    }

    // Returns current playhead in ticks (for rendering the playhead line)
    [[nodiscard]] double getCurrentPositionTicks() const {
        return m_playheadMs / EventCompiler::msPerTick(TimeData::instance().getBPM());
    }

    // Seek: set playhead to a tick position
    void seekToTicks(uint32_t ticks) {
        const double mpt = EventCompiler::msPerTick(TimeData::instance().getBPM());
        m_playheadMs = ticks * mpt;
        m_eventIndex = firstEventAtOrAfter(m_playheadMs);
        if (isTimerRunning())
            m_wallClockStart = juce::Time::getMillisecondCounterHiRes() - m_playheadMs;
    }

private:
    // ---------------------------------------------------------------
    void timerCallback() override {
        m_playheadMs = juce::Time::getMillisecondCounterHiRes() - m_wallClockStart;

        // Dispatch every event whose time has arrived
        while (m_eventIndex < m_events.size() &&
               m_events[m_eventIndex].absoluteTimeMs <= m_playheadMs)
        {
            sendEvent(m_events[m_eventIndex].message);
            ++m_eventIndex;
        }

        // End of sequence
        if (m_eventIndex >= m_events.size()) {
            allNotesOff();
            if (m_playingPtr) *m_playingPtr = false;
            m_playheadMs = 0.0;
            m_eventIndex = 0;
            stopTimer();
        }
    }

    void sendEvent(const juce::MidiMessage& msg) {
        m_audio.addMidiMessage(msg);
        if (m_midiOut) m_midiOut->sendMessageNow(msg);
    }

    void allNotesOff() {
        for (int ch = 1; ch <= 16; ++ch) {
            auto msg = juce::MidiMessage::allNotesOff(ch);
            m_audio.addMidiMessage(msg);
            if (m_midiOut) m_midiOut->sendMessageNow(msg);
        }
    }

    size_t firstEventAtOrAfter(double ms) const {
        // Binary search since m_events is sorted
        size_t lo = 0, hi = m_events.size();
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (m_events[mid].absoluteTimeMs < ms) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    void setPlaying(bool playing) {
        if (m_playingPtr) {
            *m_playingPtr = playing;
        }
    }

    bool isPlaying() const {
        return m_playingPtr ? *m_playingPtr : false;
    }
    // ---------------------------------------------------------------
    AudioManager&                  m_audio;
    std::unique_ptr<juce::MidiOutput> m_midiOut;
    std::vector<ScheduledEvent>    m_events;
    size_t                         m_eventIndex{0};
    double                         m_playheadMs{0.0};
    double                         m_wallClockStart{0.0};
    bool*                          m_playingPtr{nullptr};
};