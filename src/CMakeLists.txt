juce_add_gui_app(${PROJECT_NAME}
        PRODUCT_NAME ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PRIVATE
        MIDI_Logic/MIDI_Events/MidiEvent.cpp
        MIDI_Logic/MidiByteReader.cpp
        MIDI_Logic/ParsedMidi.cpp
        MIDI_Logic/TrackChunk.cpp
        Project_Logic/Pattern.cpp
        SetupAssets.cpp
        Main.cpp)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        juce::juce_gui_extra
        juce::juce_opengl
        juce::juce_audio_devices
        juce::juce_audio_basics
        imgui
        imgui_impl_juce
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
