#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>
#include "UI/ControlPannel/ControlComponent.h"
#include  "MIDI_Logic/ParsedMidi.h"
#include "UI/Top_nav/TopNavComponent.h"
#include "UI/Arranger/Arranger.h"
#include "Theme.h"
#include <imgui.h>
#include "backends/imgui_impl_opengl3.h"

#include <imgui_impl_juce/imgui_impl_juce.h>
#include <juce_opengl/juce_opengl.h>
#include  "UI/Piano_Roll/PianoRollMain.h"

struct windowStateM {
    bool pianoRollWindow{true};
    bool setttingsWindow{false};
};

class MainComponent
  : public juce::Component
  , public juce::OpenGLRenderer
{
public:
    TopNavComponent top_nav_component;
    ControlComponent control_component;
    Arranger arranger_component;
    PianoRollMain pianoRollMain;
    windowStateM state;

    MainComponent()
    {
        setOpaque(true);
        setSize(1920, 1080);
        setWantsKeyboardFocus(true);
        glctx.setOpenGLVersionRequired(juce::OpenGLContext::openGL3_2);
        glctx.setRenderer(this);
        glctx.attachTo(*this);
        glctx.setContinuousRepainting(true);
    }

    ~MainComponent() {
        glctx.detach();
    }

    void newOpenGLContextCreated() override
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplJuce_Init(*this, glctx);
        ImGui_ImplOpenGL3_Init();
    }

    void renderOpenGL() override
    {
        using namespace juce::gl;
        Theme::applyTheme();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplJuce_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize));

        ImGui::Begin("Main Window", nullptr, flags);

        top_nav_component.nav();

        control_component.ControldPanel(state);

        arranger_component.arranger();
        if (state.pianoRollWindow) {
            pianoRollMain.create();
        }
        else if (state.setttingsWindow) {

        }

        ImGui::End();

        ImGui::Render();

        // background begin
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // background end

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void openGLContextClosing() override
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplJuce_Shutdown();
        ImGui::DestroyContext();
    }


private:
    juce::OpenGLContext glctx;
    const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoMove
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoBringToFrontOnFocus;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
  };
