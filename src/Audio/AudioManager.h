#pragma once
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "SineWave.h"
#include <vector>


struct ScheduledEvent;

struct Instruments {
    uint32_t ID{0};
    size_t trackIndex{0};
    std::string name{"default"};
    std::unique_ptr<juce::Synthesiser> synth;
    uint8_t voices{8};// max instance of notes playing at once
    juce::AudioBuffer<float> audioBuffer;
    juce::MidiBuffer midiBuffer;

    Instruments(uint32_t ID, std::string name, uint8_t voices) {
        this->ID = ID;
        this->name = name;
        this->voices = voices;
        synth = std::make_unique<juce::Synthesiser>();
    }
};

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
        for (auto& instrument : InstrumentList) {
            instrument->midiBuffer.clear();
        }

        deviceManager.removeAudioCallback(&player);
        player.setSource(nullptr);
        deviceManager.closeAudioDevice();
        std::cout << "AudioManager shutdown" << std::endl;
    }

    void prepareToPlay(int samplesPerBlock, double sampleRate) override {
        InstrumentList.push_back(std::make_unique<Instruments>(assignInstrumentId(),
                    "sineWave",8));

        currentSampleRate = sampleRate;
        InstrumentList.back()->synth->setCurrentPlaybackSampleRate(currentSampleRate);
        InstrumentList.back()->synth->addSound(new SineWaveSound());

        for (int i = 0; i < 8; i++) {
            InstrumentList.back()->synth->addVoice(new SineWaveVoice());
        }

        createAudioDir();
        juce::File soundsDir = juce::File::getCurrentWorkingDirectory().getChildFile("sounds");

        juce::Array<juce::File> files;
        soundsDir.findChildFiles(files,juce::File::findFiles ,false, "*.wav;*.aif;*.aiff;*.mp3;*.flac;*.ogg");
        for (auto& file : files) {
            loadSample(file);
        }
    }


    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;


    void releaseResources() override {
        for (auto& buffer : InstrumentList) {
            buffer->midiBuffer.clear();
        }

    }

    std::unordered_set<juce::String> usedNames;
    void loadSample(const juce::File& audioFile) {
        juce::String name = audioFile.getFileNameWithoutExtension();
        if (usedNames.find(name)!=usedNames.end()) {
            name = name + "_" + std::to_string(InstrumentList.size());
        }
        usedNames.insert(name);
        InstrumentList.push_back(std::make_unique<Instruments>(assignInstrumentId(),
            name.toStdString(),8));

        for (int i = 0; i < 8; i++)
            InstrumentList.back()->synth->addVoice(new juce::SamplerVoice());

        juce::File copyPlace("sounds/" + name);
        audioFile.copyFileTo(copyPlace);
        juce::AudioFormatManager formatManager;

        formatManager.registerBasicFormats();

        auto* reader = formatManager.createReaderFor(audioFile);

        if (reader) {
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);

            InstrumentList.back()->synth->addSound(new juce::SamplerSound(
                name,
                *reader,
                allNotes,
                60,
                0.01,
                0.1,
                10.0
            ));
            delete reader;
        }
        InstrumentList.back()->synth->setCurrentPlaybackSampleRate(currentSampleRate);
    }

    void addMidiMessage(const ScheduledEvent& message);

    juce::Synthesiser* getSynth_ptr() { return InstrumentList.back()->synth.get(); }

    std::vector<std::unique_ptr<Instruments>>* getInstrumentList_ptr() { return &InstrumentList; }
    std::vector<std::unique_ptr<Instruments>> InstrumentList;

    Instruments* getInstrumentBasedOnTrack(size_t index) {
        for (auto& instrument : InstrumentList) {
            if (instrument->trackIndex == index) {
                return instrument.get();
            }
        }
        std::cout << "Instrument not found" << std::endl;
        return nullptr;
    }
private:
    juce::CriticalSection midiLock;
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer player;

        std::array<juce::MidiBuffer, 16> midiBuffers;

    double currentSampleRate = 44100.0;

    uint32_t m_nextInstrumentId{0};
    uint32_t assignInstrumentId() { return m_nextInstrumentId++; }

    void createAudioDir() {
        juce::File soundsDir = juce::File::getCurrentWorkingDirectory().getChildFile("sounds");
        if (!soundsDir.exists())
            soundsDir.createDirectory();
    }

};