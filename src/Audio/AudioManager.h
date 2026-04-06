#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include "SineWave.h"

class AudioManager : public juce::AudioSource {
public:
    AudioManager() {
        deviceManager.initialiseWithDefaultDevices(0, 2);
        player.setSource(this);
        deviceManager.addAudioCallback(&player);
    }

    ~AudioManager() {
        deviceManager.removeAudioCallback(&player);
        player.setSource(nullptr);
    }

    void prepareToPlay(int samplesPerBlock, double sampleRate) override {
        synth.setCurrentPlaybackSampleRate(sampleRate);
        synth.addSound(new SineWaveSound());

        for (int i = 0; i < 8; i++) {
            synth.addVoice(new SineWaveVoice());
        }
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override {
        bufferToFill.clearActiveBufferRegion();

        synth.renderNextBlock(*bufferToFill.buffer, midiBuffer,
                              bufferToFill.startSample,
                              bufferToFill.numSamples);

        midiBuffer.clear();
    }

    void releaseResources() override {
        midiBuffer.clear();
    }

    void addMidiMessage(const juce::MidiMessage& message) {
        midiBuffer.addEvent(message, 0);
    }

    juce::Synthesiser& getSynth() { return synth; }

private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer player;
    juce::Synthesiser synth;
    juce::MidiBuffer midiBuffer;
};