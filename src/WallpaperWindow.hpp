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

private:
    GtkWidget *mainWindow;
    GtkWidget *hBox;
    GtkWidget *drawingArea;
};
} // namespace aw
