#include <gst/video/video.h>
#include <glib.h>
#include <QDebug>
#include "ibuscallback.h"

/*Callback function for handling messages from the bus*/

gboolean bus_callback(GstBus *bus, GstMessage *message, gpointer data = nullptr)
    {
        //qDebug() << "Recived message: " << GST_MESSAGE_TYPE(message);
        IBusCallback* iBusCallback = static_cast<IBusCallback*>(data);

        switch (GST_MESSAGE_TYPE(message)){
        case GST_MESSAGE_ERROR:
        {
            qDebug() << "GST_MESSAGE_ERROR:";
            GError *err;
            gchar *debug;           
            gst_message_parse_error(message, &err, &debug);
            qDebug() << "Error:  " << err->message;
            g_error_free(err);
            g_free(debug);
            iBusCallback->stopLoop();
            break;
        }
        case GST_MESSAGE_EOS:
            qDebug() << "GST_MESSAGE_EOS:";
            iBusCallback->stopLoop();
            break;
        default:
            break;
        }
        return true;
    }
