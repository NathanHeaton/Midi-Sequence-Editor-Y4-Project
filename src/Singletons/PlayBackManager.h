//
// Created by nathan on 18/01/2026.
//
#pragma once
#include "../Audio/AudioManager.h"
#include "../Audio/MidiPlayer.h"

class PlayBackManager {
    public:

    static PlayBackManager& instance() {
        static PlayBackManager instance;
        return instance;
    }

    [[nodiscard]] bool isPlaying() const {
        return m_playing;
    }

    void togglePlay() {
        m_playing = !m_playing;
        if (m_playing) {player.playCurrentPattern();}
        else{ player.pauseCurrentPattern();}
    }
    void setPlaying(bool playing) {
        m_playing = playing;

    }
    private:

    PlayBackManager() {
        player.setPlayingPtr(&m_playing);
    };
    bool m_playing{false};
    AudioManager m_audioManager;
    MidiPlayer player = MidiPlayer(m_audioManager);

};
