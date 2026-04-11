#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "SineWave.h"

class AudioManager : public juce::AudioSource {
public:
    AudioManager() {
        deviceManager.initialiseWithDefaultDevices(0, 2);
        player.setSource(this);
        deviceManager.addAudioCallback(&player);

    }

    ~AudioManager() {
        // deviceManager.removeAudioCallback(&player);
        // player.setSource(nullptr);
        // deviceManager.closeAudioDevice();
    }

    void shutdownAudio() {
        midiBuffer.clear();
        deviceManager.removeAudioCallback(&player);
        player.setSource(nullptr);
        deviceManager.closeAudioDevice();
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
        juce::MidiBuffer localBuffer;
        {
            const juce::ScopedLock sl(midiLock);
            localBuffer = midiBuffer;
            midiBuffer.clear();
        }
        synth.renderNextBlock(*bufferToFill.buffer, localBuffer,
                              bufferToFill.startSample,
                              bufferToFill.numSamples);
    }

    void releaseResources() override {
        midiBuffer.clear();
    }

    void loadSample(const juce::File& audioFile) {
        // Temp TODO: added synth vector
        synth.clearSounds();
        synth.clearVoices();

        for (int i = 0; i < 8; i++)
            synth.addVoice(new juce::SamplerVoice());

        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        auto* reader = formatManager.createReaderFor(audioFile);
        if (reader) {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);

            synth.addSound(new juce::SamplerSound(
                "temp",
                *reader,
                allNotes,
                60,
                0.01,
                0.1,
                10.0
            ));
            delete reader;
        }
    }

    void addMidiMessage(const juce::MidiMessage& message) {
        const juce::ScopedLock sl(midiLock);
        midiBuffer.addEvent(message, 0);
    }

    juce::Synthesiser& getSynth() { return synth; }

private:
    juce::CriticalSection midiLock;
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer player;
    juce::Synthesiser synth;
    juce::MidiBuffer midiBuffer;
};