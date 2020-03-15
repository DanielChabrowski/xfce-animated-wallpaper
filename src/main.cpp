#include <iostream>

#include <gdk/gdkx.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <gtk/gtk.h>

struct Settings
{
    int monitor;
    gchar *path;
};

class WallpaperWindow
{
public:
    WallpaperWindow(Settings s)
    : settings{ std::move(s) }
    {
    }

    int Initialize();
    int Play();

    Settings settings;
    GstElement *play;
    GtkWidget *mainwindow;
    GtkWidget *hBox;
    GtkWidget *drawingarea;
};

int WallpaperWindow::Play()
{
    std::cout << "Playing: " << settings.path << std::endl;

    GMainLoop *loop;
    GstBus *bus;

    loop = g_main_loop_new(NULL, FALSE);

    play = gst_element_factory_make("playbin", "play");
    g_object_set(G_OBJECT(play), "uri", settings.path, NULL);
    g_object_set(G_OBJECT(play), "mute", true, NULL);

    bus = gst_pipeline_get_bus(GST_PIPELINE(play));
    gst_object_unref(bus);

    GstElement *x_overlay = gst_element_factory_make("xvimagesink", "videosink");
    g_object_set(G_OBJECT(play), "video-sink", x_overlay, NULL);
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(x_overlay),
                                        GDK_WINDOW_XID(gtk_widget_get_parent_window(drawingarea)));

    gst_element_set_state(play, GST_STATE_PLAYING);

    g_main_loop_run(loop);

    gst_element_set_state(play, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(play));

    gtk_widget_show_all(mainwindow);
    gtk_widget_realize(drawingarea);

    return 0;
}

int WallpaperWindow::Initialize()
{
    mainwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(mainwindow), 0);

    GtkWindow *window = GTK_WINDOW(mainwindow);
    gtk_window_set_default_size(window, 1920, 1080);
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
    gtk_window_move(window, 1920 * settings.monitor, 0);

    GdkDisplay *display = gdk_display_get_default();
    const auto monitorCount = gdk_display_get_n_monitors(display);
    std::cout << "monitor count: " << monitorCount << std::endl;

    for(int i = 0; i < monitorCount; ++i)
    {
        GdkMonitor *monitor = gdk_display_get_monitor(display, i);

        GdkRectangle rect;
        gdk_monitor_get_geometry(monitor, &rect);

        std::cout << "Monitor " << i << " pos: " << rect.x << 'x' << rect.y
                  << " size: " << rect.width << 'x' << rect.height << std::endl;
    }

    g_signal_connect(G_OBJECT(mainwindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    drawingarea = gtk_drawing_area_new();

    hBox = gtk_box_new(GtkOrientation::GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(hBox), drawingarea, false, true, 0);

    gtk_container_add(GTK_CONTAINER(mainwindow), hBox);

    gtk_widget_show_all(mainwindow);
    gtk_widget_realize(drawingarea);

    return 0;
}

Settings parse_settings(int argc, char *argv[])
{
    Settings s;

    static GOptionEntry entries[] = { { "monitor", 'm', 0, G_OPTION_ARG_INT, &s.monitor, "Monitor index", "N" },
                                      { "wallpaper", 'w', 0, G_OPTION_ARG_STRING, &s.path, "Wallpaper path", "M" },
                                      { NULL } };

    GOptionContext *context = g_option_context_new("xfce-animated-wallpaper");
    g_option_context_add_main_entries(context, entries, NULL);
    g_option_context_add_group(context, gtk_get_option_group(TRUE));

    GError *error = NULL;
    if(!g_option_context_parse(context, &argc, &argv, &error))
    {
        g_print("option parsing failed: %s\n", error->message);
        std::exit(1);
    }

    return s;
}

int main(int argc, char *argv[])
{
    int *x = new int;
    (void)x;

    gtk_init(&argc, &argv);
    gst_init(&argc, &argv);

    const auto settings = parse_settings(argc, argv);

    WallpaperWindow topwin{ settings };

    if(topwin.Initialize() == 0)
    {
        g_timeout_add(
            1,
            [](gpointer data) -> int {
                ((WallpaperWindow *)data)->Play();
                return 0;
            },
            &topwin);

        gtk_main();
    }

    return 0;
}
