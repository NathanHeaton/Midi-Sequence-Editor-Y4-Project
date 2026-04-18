#pragma once
#include "../Audio/AudioManager.h"
#include "../Audio/MidiPlayer.h"
#include "../Audio/NotePreview.h"

enum class PlaybackMode { PianoRoll, Arranger };

class PlayBackManager {
public:

    static PlayBackManager& instance() {
        static PlayBackManager inst;
        return inst;
    }

    [[nodiscard]] bool    isPlaying()               const { return m_playing; }
    [[nodiscard]] double  getPlayheadPositionTicks() const {
     return player.getCurrentPositionTicks();
}

    [[nodiscard]] double getArrangerPlayheadTicks () {
        return player.getArrangerPositionTicks();
    }
    [[nodiscard]] double getPianoRollPlayheadTicks () {
        return player.getPianoRollPositionTicks();
    }

    void setMode(PlaybackMode mode) { m_mode = mode; }
    [[nodiscard]] PlaybackMode getMode() const { return m_mode; }

    void togglePlay() {
        m_playing = !m_playing;
        if (m_playing) {
            auto src = (m_mode == PlaybackMode::Arranger)
                ? PlaybackSource::Arranger
                : PlaybackSource::PianoRoll;
            player.play(src);
        } else {
            player.pause();
        }
    }

    void stop() {
        m_playing = false;
        player.stop();
    }

    void seekToTicks(uint32_t ticks, bool isArranger) {
        isArranger ?
        player.seekSourceToTicks(PlaybackSource::Arranger, ticks) :
        player.seekSourceToTicks(PlaybackSource::PianoRoll, ticks);

    }

    void init() {
        player.setPlayingPtr(&m_playing);
        juce::MessageManager::callAsync([this]() {
            player.initialiseMidiPlayer();
        });
    }

    void addSoundFile(juce::File& soundFile) {
        m_audioManager.loadSample(soundFile);
    }

    void shutDown() {
        m_audioManager.shutdownAudio();
        player.cleanUp();
    }

    void playOnNote(size_t track, uint8_t pitch, bool hold){
        ScheduledEvent event;
        event.message = juce::MidiMessage::noteOn(1, pitch, static_cast<uint8_t>(127));
        event.trackIndex = track;
        player.sendEvent(event);
        if (!hold) notePreviewTimeout.start(track, pitch);
    }

    void playOffNote(size_t track, uint8_t pitch){
        ScheduledEvent offEvent;
        offEvent.message = juce::MidiMessage::noteOff(1, pitch, static_cast<uint8_t>(127));
        offEvent.trackIndex = track;
        player.sendEvent(offEvent);
    }



    void toggleLooping(){ player.looping = !player.looping;}

    Instruments* getInstrumentByID(uint32_t ID) {
        auto instruments = m_audioManager.getInstrumentList_ptr();

        for (auto& instrument : *instruments) {
            if (instrument->ID == ID) {
                return instrument.get();
            }
        }
        return nullptr;
    }

    std::vector<std::unique_ptr<Instruments>>* getInstruments() {
        return m_audioManager.getInstrumentList_ptr();
    }

private:
    PlayBackManager(){
        notePreviewTimeout.sendNoteOff = [this]() { playOffNote(notePreviewTimeout.trackIndex, notePreviewTimeout.pitch); };
    };
    PlaybackMode  m_mode{PlaybackMode::Arranger};
    bool          m_playing{false};
    AudioManager  m_audioManager;
    NotePreview   notePreviewTimeout;
    MidiPlayer    player{m_audioManager};
};