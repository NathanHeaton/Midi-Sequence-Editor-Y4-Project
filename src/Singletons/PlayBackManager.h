#pragma once
#include "../Audio/AudioManager.h"
#include "../Audio/MidiPlayer.h"

enum class PlaybackMode { PianoRoll, Arranger };

class PlayBackManager {
public:
    static PlayBackManager& instance() {
        static PlayBackManager inst;
        return inst;
    }

    [[nodiscard]] bool    isPlaying()               const { return m_playing; }
    [[nodiscard]] double  getPlayheadPositionTicks() const { return player.getCurrentPositionTicks(); }

    void shutDown() { player.cleanUp(); }

    void setMode(PlaybackMode mode) { m_mode = mode; }

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

    void seekToTicks(uint32_t ticks) {
        player.seekToTicks(ticks);
    }

    void setPlaybackMode(PlaybackMode m) {m_mode = m;}

private:
    PlayBackManager() {
        player.setPlayingPtr(&m_playing);
        juce::MessageManager::callAsync([this]() {
            player.initialiseMidiPlayer();
        });
    }

    PlaybackMode  m_mode{PlaybackMode::Arranger};
    bool          m_playing{false};
    AudioManager  m_audioManager;
    MidiPlayer    player{m_audioManager};
};