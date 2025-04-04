#ifndef BUS_CALLBACK_H
#define BUS_CALLBACK_H
#include <gst/video/video.h>
#include <glib.h>

gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer data = nullptr);

#endif // BUS_CALLBACK_H
