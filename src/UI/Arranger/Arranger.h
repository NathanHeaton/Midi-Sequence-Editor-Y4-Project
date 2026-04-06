#pragma once

#include <memory>

#include "../../Theme.h"
#include "TopControls.h"
#include "TrackControls.h"
#include "ArrTimeline.h"
#include "../../Singletons/ViewState.h"
#include "../../Singletons/ProjectData.h"
#include "../../Singletons/ArrangerManager.h"
#include "../Common/Toolbar.h"


class TimelineLabel;

class Arranger
{
public:
    TopControls topControls;
    ArrTimeline arrangerTimeline;
    Toolbar arrangerToolbar;
    std::vector<std::unique_ptr<TrackControls>> tracksControls;
    const float trackControlWidth = 250.0f;

    float timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::arranger)* ProjectData::instance().getTotalBars();
    float timelineXScroll = 0.0f;
    Arranger() {
        // AddTrack();
        // arrangerToolbar.addGroup(ToolbarGroups::arrangerTools());
        // arrangerToolbar.addGroup(ToolbarGroups::playback());
        // arrangerToolbar.addGroup(ToolbarGroups::arrangerZoom());
    }


    void create();
    void AddTrack();
    private:
    std::unique_ptr<TimelineLabel> timelineLabel; // pointer so forward decl works
};
