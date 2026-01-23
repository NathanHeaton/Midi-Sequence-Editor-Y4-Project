//
// Created by idad on 23/01/2026.
//

#pragma once

#include <juce_audio_devices/juce_audio_devices.h>

class SineWaveSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};


class SineWave : public juce::SynthesiserVoice {
    public:
        bool canPlaySound(juce::SynthesiserSound* sound) override
    { return dynamic_cast<SineWaveSound*>(sound) != nullptr; }

    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override {
            currentAngle = 0.0;
            level = velocity * 0.15;

            auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
            auto cyclesPerSample = cyclesPerSecond / getSampleRate();
            angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<float>::pi;

        }

    void stopNote(float velocity, bool allowTailOff) override {
            clearCurrentNote();
            angleDelta = 0.0f;
        }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override {
        for (int i = 0; i < numSamples; ++i) {
            outputBuffer.setSample(0, startSample + i, level * std::sin(currentAngle));
            currentAngle += angleDelta;
        }
    }

    private:
    double currentAngle = 0.0f;
    double level = 0.0f;
    double angleDelta = 0.0f;
};
