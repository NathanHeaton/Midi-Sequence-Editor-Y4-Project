#pragma once
#include <functional>
#include <string>

struct ToolbarGroup {
    std::string groupId;
    std::function<void()> render;
};