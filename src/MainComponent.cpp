#include "MainComponent.h"

#include "MIDI_Logic/MidiFileManager.h"



MainComponent::MainComponent()
{
    addAndMakeVisible(arranger);
    addAndMakeVisible(testBox);
    addAndMakeVisible(openFileButton);
    addAndMakeVisible(&topNavComponent);
    addAndMakeVisible(midiMessageBox);


    test_function();
    midifileManager.addActionListener(this);

    openFileButton.addListener(this);
    setSize (1960, 1080);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::background);
    g.setFont (juce::FontOptions (45.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Test Project", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    arranger.setBounds(getLocalBounds().withHeight(getHeight()).withTrimmedTop(60).reduced(5));
    //top Nav
    topNavComponent.setBounds(getLocalBounds().withHeight(36));
    //buttons
    // testBox.setBounds(getLocalBounds().withHeight(130));
    openFileButton.setBounds(30,30, 100, 30);
    // boxes

    midiMessageBox.setBounds(getLocalBounds().withHeight(1000).reduced(10).withTrimmedTop(400));

}


void MainComponent::buttonClicked(juce::Button* button)
{
    if(button == &openFileButton)
    {
        juce::File initalDir = juce::File::getCurrentWorkingDirectory();    

        auto chooser = std::make_shared<juce::FileChooser>("choose a .midi or .mid file",
                                    initalDir ,
                                    "*.mid;*");
        
        chooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser](const juce::FileChooser& fc)
            {
                juce::File chosen = fc.getResult();
                if (chosen.existsAsFile())
                {
                    DBG("Async selected file: " + chosen.getFullPathName());
                    midifileManager.loadFile(chosen);
                }
            });

    }

}



