#pragma once

#include <gst/gst.h>
#include <gtk/gtk.h>

#include <string>

namespace aw
{
struct Monitor;

class WallpaperWindow
{
public:
    WallpaperWindow(const Monitor &);

    WallpaperWindow(const WallpaperWindow &) = delete;
    WallpaperWindow &operator=(const WallpaperWindow &) = delete;

    void applyWallpaper(const std::string &path);

private:
    GstElement *playbin;
    GtkWidget *mainWindow;
    GtkWidget *drawingArea;
};
} // namespace aw
