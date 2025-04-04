#ifndef PLAYER_H
#define PLAYER_H
#include <gst/gst.h>
#include <gst/video/video.h>
#include <glib.h>
#include <QWidget>
#include <QObject>

#include "ibuscallback.h"

class Player : public QObject, public IBusCallback
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    void stopLoop() override;
    void test() override;

public slots:
    void setRtsp(const QString& rtsp);
    void setWinId(const WId wid);
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
    WId m_wid;
    //for play_1
    GstElement *pipeline;
    GstBus *bus;
    GstMessage *msg;
    guint bus_watch_id;


    /* This function will be called by the pad-added signal */
    void pad_added_handler(GstElement* src, GstPad* new_pad, CustomData* data);

public:
    /*This variable is necessary to run the main loop */
    GMainLoop* loop;
public slots:
    void play_video();
    void pause_video();

signals:

};

#endif // PLAYER_H
