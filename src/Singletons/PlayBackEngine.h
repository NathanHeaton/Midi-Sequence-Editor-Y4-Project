//
// Created by nathan on 18/01/2026.
//


#pragma once
#include "../Audio/MidiPlayer.h"

class PlayBackEngine {
    public:

    static PlayBackEngine& instance() {
        static PlayBackEngine instance;
        return instance;
    }

    [[nodiscard]] bool isPlaying() const {
        return m_playing;
    }

    void startPlaying() {
        midiPlayer.playCurrentPattern();
        m_playing = true;
    }

    private:
    bool m_playing{false};
    MidiPlayer midiPlayer;

    PlayBackEngine() = default;

};