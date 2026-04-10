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
    [[nodiscard]] double  getPlayheadPositionTicks() const {
     return player.getCurrentPositionTicks();
}

    [[nodiscard]] double getArrangerPlayheadTicks () {
        return player.getArrangerPositionTicks();
    }
    [[nodiscard]] double getPianoRollPlayheadTicks () {
        return player.getPianoRollPositionTicks();
    }

    void shutDown() { player.cleanUp(); }

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
        std::cout << "mode" <<std::endl;
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