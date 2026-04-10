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
        if (m_midiOut) {
            DBG("MIDI Output opened: " << devices[0].name);

    } else {
        DBG("No MIDI output devices available");
    }

    }

    void setPlayingPtr(bool* ptr) { m_playingPtr = ptr; }

    void play(PlaybackSource source) {
        m_activeSource = source;
        const double bpm = TimeData::instance().getBPM();
        if (source == PlaybackSource::PianoRoll) {
            m_events = EventCompiler::compilePattern(
                *PatternManager::instance().getCurrentPattern(), bpm);
        } else {
            std::cout << "Arranger playing" <<std::endl;
            m_events = EventCompiler::compileArranger(bpm);
        }
        if (m_events.empty()) return;
        double& playheadMs = activePlayheadMs();
        m_eventIndex     = firstEventAtOrAfter(playheadMs);
        m_wallClockStart = juce::Time::getMillisecondCounterHiRes() - playheadMs;
        startTimer(1);
    }

    void pause() {
        stopTimer();
        allNotesOff();
    }

    void stop() {
        stopTimer();

        allNotesOff();
        activePlayheadMs() = 0.0;
        m_eventIndex = 0;
    }

    // Returns current playhead in ticks for the active source
    [[nodiscard]] double getCurrentPositionTicks() const {
        return activePlayheadMs() / msPerTick();
    }

    // Per-source getters (for rendering both playheads independently)
    [[nodiscard]] double getPianoRollPositionTicks() const {
        return m_pianoRollPlayheadMs / msPerTick();
    }
    [[nodiscard]] double getArrangerPositionTicks() const {
        return m_arrangerPlayheadMs / msPerTick();
    }


    // Seek the active source to a tick position
    void seekToTicks(uint32_t ticks) {
        activePlayheadMs() = ticks * msPerTick();
        m_eventIndex = firstEventAtOrAfter(activePlayheadMs());
        if (isTimerRunning())
            m_wallClockStart = juce::Time::getMillisecondCounterHiRes() - activePlayheadMs();
    }

    // Seek a specific source without changing the active one
    void seekSourceToTicks(PlaybackSource source, uint32_t ticks) {
        playheadMsFor(source) = ticks * msPerTick();
        if (m_activeSource == source) {
            m_eventIndex = firstEventAtOrAfter(activePlayheadMs());
            if (isTimerRunning())
                m_wallClockStart = juce::Time::getMillisecondCounterHiRes() - activePlayheadMs();
        }
    }

private:
    // ---------------------------------------------------------------
    void timerCallback() override {
        activePlayheadMs() = juce::Time::getMillisecondCounterHiRes() - m_wallClockStart;

        while (m_eventIndex < m_events.size() &&
               m_events[m_eventIndex].absoluteTimeMs <= activePlayheadMs())
        {
            sendEvent(m_events[m_eventIndex].message);
            ++m_eventIndex;
        }

        if (m_eventIndex >= m_events.size()) {
            allNotesOff();
            if (m_playingPtr) *m_playingPtr = false;
            activePlayheadMs() = 0.0;
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
        size_t lo = 0, hi = m_events.size();
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (m_events[mid].absoluteTimeMs < ms) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    [[nodiscard]] double msPerTick() const {
        return EventCompiler::msPerTick(TimeData::instance().getBPM());
    }

    // Non-const helpers so timerCallback can write through them
    double& activePlayheadMs() { return playheadMsFor(m_activeSource); }
    [[nodiscard]] const double& activePlayheadMs() const { return playheadMsFor(m_activeSource); }

    double& playheadMsFor(PlaybackSource source) {
        return source == PlaybackSource::PianoRoll
            ? m_pianoRollPlayheadMs
            : m_arrangerPlayheadMs;
    }
    [[nodiscard]] const double& playheadMsFor(PlaybackSource source) const {
        return source == PlaybackSource::PianoRoll
            ? m_pianoRollPlayheadMs
            : m_arrangerPlayheadMs;
    }

    bool isPlaying() const { return m_playingPtr ? *m_playingPtr : false; }

    // ---------------------------------------------------------------
    AudioManager&                     m_audio;
    std::unique_ptr<juce::MidiOutput> m_midiOut;
    std::vector<ScheduledEvent>       m_events;
    size_t                            m_eventIndex{0};
    double                            m_pianoRollPlayheadMs{0.0};
    double                            m_arrangerPlayheadMs{0.0};
    double                            m_wallClockStart{0.0};
    PlaybackSource                    m_activeSource{PlaybackSource::PianoRoll};
    bool*                             m_playingPtr{nullptr};
};