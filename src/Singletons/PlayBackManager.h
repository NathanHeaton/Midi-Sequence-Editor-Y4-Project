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

    void startPlaying() {
        player.playCurrentPattern();
        m_playing = true;
    }

    private:
    PlayBackManager();

    bool m_playing{false};
    AudioManager m_audioManager;

    MidiPlayer player = MidiPlayer(m_audioManager);



};

inline PlayBackManager::PlayBackManager() {
}
