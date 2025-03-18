#ifndef PLAYER_H
#define PLAYER_H
#include <gst/gst.h>
#include <gst/video/video.h>
#include <glib.h>

#include <QWidget>

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
public slots:
    void setRtsp(const QString& rtsp);
    void play_1();
    void play_2();
    void play_3();



private:
    QString m_rtsp;
    /* Structure to contain all our information, so we can pass it to callbacks */
    struct CustomData {
        GstElement *pipeline;
        GstElement *source;
        GstElement *convert;
        GstElement *resample;
        GstElement *sink;
    };

    /* This function will be called by the pad-added signal */
    static void pad_added_handler(GstElement* src, GstPad* new_pad, CustomData* data);
    /*Callback function for handling messages from the bus*/
    static gboolean bus_callback(GstBus* bus, GstMessage* message, gpointer data);
public:
    /*This variable is necessary to run the main loop */
    static GMainLoop* loop;


signals:

};

#endif // PLAYER_H
