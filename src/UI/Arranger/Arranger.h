#pragma once

#include <memory>

#include "../../Theme.h"

#include "TrackControls.h"
#include "ArrTimeline.h"
#include "../../Singletons/ViewState.h"
#include "../../Singletons/ProjectData.h"
#include "../../Singletons/ArrangerManager.h"
#include "../Common/Toolbar.h"
#include "../Common/ToolbarGroups.h"
#include "../Common/TimelineLabel.h"
class Arranger
{
public:
    ArrTimeline arrangerTimeline;
    Toolbar arrangerToolbar;
    TimelineLabel timelineLabel;
    std::vector<std::unique_ptr<TrackControls>> tracksControls;
    const float trackControlWidth = 250.0f;

    float timelineLength = ViewState::instance().getPixelPerBar(zoomFactor::arranger)* ProjectData::instance().getTotalBars();
    float timelineXScroll = 0.0f;
    Arranger() {
        AddTrack();
        arrangerToolbar.addGroup(ToolbarGroups::snapping(true));
        arrangerToolbar.addGroup(ToolbarGroups::arrangerTools());
        arrangerToolbar.addGroup(ToolbarGroups::arrangerZoom());
        arrangerToolbar.addPlayBackButton(ToolbarGroups::playback(true,0));
    }


    void create();
    void AddTrack();
    private:

};
