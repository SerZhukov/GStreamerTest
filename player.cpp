#include "player.h"
#include "bus_callback.h"

Player::Player(QObject *parent)
    : QThread{parent}
{

    // Initialize GStreamer
    gst_init(nullptr, nullptr);
    qDebug() << "PLayer thread: " << QThread::currentThreadId();
}

void Player::stopLoop()
{
    qDebug() << "void Player::stopLoop()";
    g_main_loop_quit(loop);
}


void Player::setRtsp(const QString &rtsp)
{
    m_rtsp = rtsp;
}

void Player::setWinId(const WId wid)
{
    m_wid = wid;
}

void Player::play_1()
{
    qDebug() << "void Player::play_1()";
    qDebug() << "Player::play_1 thread: " << QThread::currentThreadId();

     // Build the pipeline
    //pipeline = gst_parse_launch("playbin uri=file:///D:/Film/Alien.mkv", nullptr);
    pipeline = gst_parse_launch("playbin uri=rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=10&stream=0.sdp?real_stream", nullptr);
    if(pipeline)
    {
    sink = gst_bin_get_by_interface(GST_BIN(pipeline), GST_TYPE_VIDEO_OVERLAY);
    }
    else
    {
        qDebug() << "Error pipeline";
        return;
    }

    if (sink) {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), m_wid);
    }
    // Start playing
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
     // Wait until error or EOS
    bus = gst_element_get_bus(pipeline);
    bus_watch_id = gst_bus_add_watch(bus, bus_callback, this);
    gst_object_unref(bus);
    loop = g_main_loop_new(nullptr, false);
    g_main_loop_run(loop);
     // Free resources
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_source_remove (bus_watch_id);
    g_main_loop_unref (loop);
}

void Player::play_2()
{
    //for play_2
    GstElement *pipeline;
    GstElement *source;
    GstElement *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;


    //Create the elements
    source = gst_element_factory_make("videotestsrc", "source");
    sink = gst_element_factory_make("d3dvideosink", "sink");

    //Create the empty pipeline
    pipeline = gst_pipeline_new("test-pipeline");
    if (!pipeline || !source || !sink)
    {
        g_printerr("Not all elements could be created.\n");
        return;
    }

    //Build the pipeline
    gst_bin_add_many(GST_BIN(pipeline), source, sink, nullptr);
    if (gst_element_link(source, sink) != true)
    {
        g_printerr("Element could not be linked.\n");
        gst_object_unref(pipeline);
        return;
    }
    //Modify the source's properties
    g_object_set(source, "pattern", 0, nullptr);

    //Qt window integration

    if (GST_IS_VIDEO_OVERLAY(sink))
    {
        gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), m_wid);
    }
    else
    {
        qDebug() << "This sink is not support GstVideoOverlay";
    }

    //Start playing
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing set.\n");
        gst_object_unref(pipeline);
        return;
    }
    //Wait until error or EOS
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    // Parse message
    if (msg != nullptr) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error (msg, &err, &debug_info);
            g_printerr ("Error received from element %s: %s\n",
                       GST_OBJECT_NAME (msg->src), err->message);
            g_printerr ("Debugging information: %s\n",
                       debug_info ? debug_info : "none");
            g_clear_error (&err);
            g_free (debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print ("End-Of-Stream reached.\n");
            break;
        default:
            /* We should not reach here because we only asked for ERRORs and EOS */
            g_printerr ("Unexpected message received.\n");
            break;
        }
        gst_message_unref (msg);
    }
    //Free resources
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
}

void Player::play_3()
{
    CustomData data;        
    /* Create the elements */
    data.source = gst_element_factory_make ("uridecodebin", "source");
    data.convert = gst_element_factory_make ("audioconvert", "convert");
    data.resample = gst_element_factory_make ("audioresample", "resample");
    data.sink = gst_element_factory_make ("autoaudiosink", "sink");

    /* Create the empty pipeline */
    data.pipeline = gst_pipeline_new ("test-pipeline");

    if (!data.pipeline || !data.source || !data.convert || !data.resample || !data.sink) {
        g_printerr ("Not all elements could be created.\n");
        return;
    }

    /* Build the pipeline. Note that we are NOT linking the source at this
   * point. We will do it later. */
    gst_bin_add_many (GST_BIN (data.pipeline), data.source, data.convert, data.resample, data.sink, nullptr);
    if (!gst_element_link_many (data.convert, data.resample, data.sink, nullptr)) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (data.pipeline);
        return;
    }

    /* Set the URI to play */
    g_object_set (data.source, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", nullptr);

    /* Connect to the pad-added signal */
    g_signal_connect (data.source, "pad-added", G_CALLBACK (pad_added_handler), &data);

    /* Start playing */
    GstStateChangeReturn ret;
    ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (data.pipeline);
        return;
    }
    /* Listen to the bus */
    GstBus *bus;
    bus = gst_element_get_bus (data.pipeline);
    gboolean terminate = false;
    GstMessage *msg;
    do {
        msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
                                         static_cast<GstMessageType>(GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

        /* Parse message */
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE (msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error (msg, &err, &debug_info);
                g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
                g_clear_error (&err);
                g_free (debug_info);
                terminate = true;
                break;
            case GST_MESSAGE_EOS:
                g_print ("End-Of-Stream reached.\n");
                terminate = true;
                break;
            case GST_MESSAGE_STATE_CHANGED:
                /* We are only interested in state-changed messages from the pipeline */
                if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data.pipeline)) {
                    GstState old_state, new_state, pending_state;
                    gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
                    g_print ("Pipeline state changed from %s to %s:\n",
                            gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
                }
                break;
            default:
                /* We should not reach here */
                g_printerr ("Unexpected message received.\n");
                break;
            }
            gst_message_unref (msg);
        }
    } while (!terminate);
    /* Free resources */
    gst_object_unref (bus);
    gst_element_set_state (data.pipeline, GST_STATE_NULL);
    gst_object_unref (data.pipeline);

}

void Player::run()
{
    qDebug() << "PLayer::run thread: " << QThread::currentThreadId();
    play_3();
}

void Player::pad_added_handler(GstElement *src, GstPad *new_pad, CustomData *data)
{
        GstPad *sink_pad = gst_element_get_static_pad (data->convert, "sink");
        GstPadLinkReturn ret;
        GstCaps *new_pad_caps = nullptr;
        GstStructure *new_pad_struct = nullptr;
        const gchar *new_pad_type = nullptr;

        g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));

        // If our converter is already linked, we have nothing to do here
        if (gst_pad_is_linked (sink_pad)) {
            g_print ("We are already linked. Ignoring.\n");
            // Unreference the sink pad
            gst_object_unref (sink_pad);
            return;
        }

        // Check the new pad's type
        new_pad_caps = gst_pad_get_current_caps (new_pad);
        new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
        new_pad_type = gst_structure_get_name (new_pad_struct);
        if (!g_str_has_prefix (new_pad_type, "audio/x-raw")) {
            g_print ("It has type '%s' which is not raw audio. Ignoring.\n", new_pad_type);
            // Unreference the new pad's caps, if we got them
            if (new_pad_caps != nullptr)
                gst_caps_unref (new_pad_caps);
            // Unreference the sink pad
            gst_object_unref (sink_pad);
            return;
        }
        // Attempt the link
        ret = gst_pad_link (new_pad, sink_pad);
        if (GST_PAD_LINK_FAILED (ret)) {
            g_print ("Type is '%s' but link failed.\n", new_pad_type);
        } else {
            g_print ("Link succeeded (type '%s').\n", new_pad_type);
        }
        // Unreference the new pad's caps, if we got them
        if (new_pad_caps != NULL)
            gst_caps_unref (new_pad_caps);
        // Unreference the sink pad
        gst_object_unref (sink_pad);
}


    void Player::play_video()
    {
        gst_element_set_state(pipeline, GST_STATE_PLAYING);
    }

    void Player::pause_video()
    {
        qDebug() << "void Player::pause_video()";
        gst_element_set_state(pipeline, GST_STATE_PAUSED);
    }



