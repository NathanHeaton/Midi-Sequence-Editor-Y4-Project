#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_events/juce_events.h>
#include "UI/ControlPannel/ControlComponent.h"
#include  "MIDI_Logic/ParsedMidi.h"
#include "UI/Top_nav/TopNavComponent.h"
#include "UI/Arranger/Arranger.h"
#include "Theme.h"
#include "backends/imgui_impl_opengl3.h"
#include <imgui_impl_juce/imgui_impl_juce.h>
#include <juce_opengl/juce_opengl.h>

#include <imgui.h>
#include  "UI/Piano_Roll/PianoRollMain.h"
#include "Singletons/PanelManager.h"


class MainComponent
  : public juce::Component
  , public juce::OpenGLRenderer
{
public:
    TopNavComponent top_nav_component;
    ControlComponent control_component;
    Arranger arranger_component;
    std::deque<PianoRollMain> pianoRollPanels;

    MainComponent() {
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

    void newOpenGLContextCreated() override {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplJuce_Init(*this, glctx);
        ImGui_ImplOpenGL3_Init();
        ASSETS.LoadAll(); // after glcontext is setup
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
    }
    // stop juce assertion failure of juce::component:1695
    void paint(juce::Graphics& g) override {}

    void renderOpenGL() override {
        using namespace juce::gl;

        Theme::applyTheme();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplJuce_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize));

        ImGui::Begin("Main Window", nullptr, flags);

        top_nav_component.nav();
        control_component.ControlPanel();
        arranger_component.create();


        if (PanelManager::instance().newPaternPanelJustCreated) {
            pianoRollPanels.emplace_back(PanelManager::instance().getPanels().at(
                PanelManager::instance().getPanels().size()-1));
            PanelManager::instance().newPaternPanelJustCreated = false;
        }

        for (auto& p : pianoRollPanels){
            if (p.panelDetails->open) {
                if (PatternManager::instance().getPatternByID(p.panelDetails->patternID)==nullptr) {
                    std::cout<<"can't find pattern of that ID"<<std::endl;
                    continue;
                }
                p.create();
            }
        }

        ImGui::End();
        ImGui::Render();
        // background begin
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // background end
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void openGLContextClosing() override {
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
