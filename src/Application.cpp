#include "Application.hpp"
#include "Options.hpp"
#include "WallpaperWindow.hpp"

#include <gdk/gdkx.h>
#include <gst/gst.h>
#include <gtk/gtk.h>

namespace
{
aw::Options parseCommandLineOptions(int argc, char *argv[])
{
    aw::Options o;

    const char *wallpaperPath{ nullptr };

    static GOptionEntry entries[] = {
        { "wallpaper", 'w', 0, G_OPTION_ARG_STRING, &wallpaperPath, "Wallpaper path", "M" }, { NULL }
    };

    GOptionContext *context = g_option_context_new("xfce-animated-wallpaper");
    g_option_context_add_main_entries(context, entries, nullptr);
    g_option_context_add_group(context, gtk_get_option_group(true));

    GError *error = NULL;
    if(!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_error("option parsing failed: %s\n", error->message);
        std::exit(1);
    }

    o.wallpaperPath = std::string{ wallpaperPath };

    return o;
}
} // namespace

namespace aw
{
Application::Application(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    gst_init(&argc, &argv);

    g_message("Starting application");

    options = parseCommandLineOptions(argc, argv);
    g_message("Wallpaper path: %s", options.wallpaperPath.c_str());

    loadMonitors();
}

int Application::run()
{
    createWindows();

    for(auto &window : windows)
    {
        window->applyWallpaper("file://" + options.wallpaperPath);
    }

    gtk_main();
    return 0;
}

void parseCommandLineOptions(int argc, char *argv[]);

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
