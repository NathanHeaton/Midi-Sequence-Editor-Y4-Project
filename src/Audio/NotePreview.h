//
// Created by idad on 18/04/2026.
//
#include <juce_audio_devices/juce_audio_devices.h>

#ifndef MYPROJECT_NOTEPREVIEW_H
#define MYPROJECT_NOTEPREVIEW_H

#endif //MYPROJECT_NOTEPREVIEW_H

class NotePreview : public juce::Timer
{
public:
    NotePreview() = default;

    void shutdown() {
        stopTimer();
    }

    void start(size_t t,uint8_t p) {
        if (isTimerRunning()){
            sendNoteOff();
            stopTimer();
        }

        trackIndex = t;
        pitch = p;
        startTimer(300);
    }

    uint8_t pitch{60};
    size_t trackIndex{0};

    std::function<void()> sendNoteOff;

private:
    void timerCallback() override {
        sendNoteOff();
        stopTimer();
    }
};