#include "WallpaperWindow.hpp"
#include "Monitor.hpp"

#include <gdk/gdkx.h>
#include <gst/video/videooverlay.h>

namespace aw
{
WallpaperWindow::WallpaperWindow(const Monitor &monitor)
{
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 0);

    GtkWindow *window = GTK_WINDOW(mainWindow);
    gtk_window_set_default_size(window, monitor.width, monitor.height);
    gtk_window_set_resizable(window, false);
    gtk_window_set_screen(window, gdk_screen_get_default());
    gtk_window_stick(window);
    gtk_window_set_keep_below(window, true);
    gtk_window_set_decorated(window, false);
    gtk_window_set_type_hint(window, GdkWindowTypeHint::GDK_WINDOW_TYPE_HINT_DESKTOP);
    gtk_window_set_skip_taskbar_hint(window, true);
    gtk_window_set_skip_pager_hint(window, true);
    gtk_window_set_focus_on_map(window, false);
    gtk_window_set_accept_focus(window, false);
    gtk_window_move(window, monitor.x, monitor.y);

    drawingArea = gtk_drawing_area_new();

    hBox = gtk_box_new(GtkOrientation::GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hBox), drawingArea, false, true, 0);

    gtk_container_add(GTK_CONTAINER(mainWindow), hBox);

    gtk_widget_show_all(mainWindow);
    gtk_widget_realize(drawingArea);

    playbin = gst_element_factory_make("playbin", "play");
    g_object_set(G_OBJECT(playbin), "mute", true, NULL);

    // soft-colorbalance+deinterlace+video
    constexpr unsigned int flags = 0x00000001 | 0x00000200 | 0x00000400;
    g_object_set(G_OBJECT(playbin), "flags", flags, NULL);

    GstElement *x_overlay = gst_element_factory_make("xvimagesink", "videosink");
    g_object_set(G_OBJECT(playbin), "video-sink", x_overlay, NULL);

    // GstElement *rateFilter = gst_element_factory_make("videorate", "videorate");
    // g_object_set(G_OBJECT(rateFilter), "max-rate", 2, NULL);
    // g_object_set(G_OBJECT(playbin), "video-filter", rateFilter, NULL);

    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(x_overlay),
                                        GDK_WINDOW_XID(gtk_widget_get_parent_window(drawingArea)));
}

void WallpaperWindow::applyWallpaper(const std::string &path)
{
    g_message("Applying wallpaper: %s", path.c_str());

    g_object_set(G_OBJECT(playbin), "uri", path.c_str(), NULL);
    gst_element_set_state(playbin, GST_STATE_PLAYING);
}
} // namespace aw
