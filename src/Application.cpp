#include "Application.hpp"
#include "WallpaperWindow.hpp"

#include <gdk/gdkx.h>
#include <gst/gst.h>
#include <gtk/gtk.h>

namespace aw
{
Application::Application(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    gst_init(&argc, &argv);

    g_message("Starting application");

    loadMonitors();
}

int Application::run()
{
    createWindows();
    gtk_main();
    return 0;
}

void Application::loadMonitors()
{
    g_message("Loading monitors");

    GdkDisplay *display = gdk_display_get_default();
    const auto monitorCount = gdk_display_get_n_monitors(display);

    for(int i = 0; i < monitorCount; ++i)
    {
        GdkMonitor *monitor = gdk_display_get_monitor(display, i);

        GdkRectangle rect;
        gdk_monitor_get_geometry(monitor, &rect);

        monitors.emplace_back(Monitor{ rect.x, rect.y, rect.width, rect.height });

        g_message("Monitor %d size: %dx%d pos: %dx%d", i, rect.width, rect.height, rect.x, rect.y);
    }
}

void Application::createWindows()
{
    g_message("Create windows");

    for(const auto &monitor : monitors)
    {
        windows.emplace_back(std::make_unique<WallpaperWindow>(monitor));
    }
}
} // namespace aw
