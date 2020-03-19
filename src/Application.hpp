#pragma once

#include "Monitor.hpp"
#include "Options.hpp"
#include "WallpaperWindow.hpp"

#include <memory>
#include <vector>

namespace aw
{
class Application final
{
public:
    Application(int argc, char *argv[]);

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    int run();

private:
    void loadMonitors();
    void createWindows();

private:
    Options options;
    std::vector<Monitor> monitors;
    std::vector<std::unique_ptr<WallpaperWindow>> windows;
};
} // namespace aw
