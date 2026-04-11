#pragma once
#include <functional>
#include <string>

struct ToolbarGroup {
    std::string groupId;
    std::function<void(bool)> render;
};

struct PlayBack {
    std::string groupId;
    std::function<void(bool, uint32_t)> renderP;
};
