#include <QApplication>
#include "handlergst.h"


QString rtsp_url_1 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=1&stream=0.sdp?real_stream";
QString rtsp_url_2 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=2&stream=0.sdp?real_stream";
QString rtsp_url_3 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=3&stream=0.sdp?real_stream";
QString rtsp_url_4 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=4&stream=0.sdp?real_stream";
QString rtsp_url_5 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=5&stream=0.sdp?real_stream";
QString rtsp_url_6 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=6&stream=0.sdp?real_stream";
QString rtsp_url_7 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=7&stream=0.sdp?real_stream";
QString rtsp_url_8 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=9&stream=0.sdp?real_stream";
QString rtsp_url_9 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=10&stream=0.sdp?real_stream";
QString rtsp_url_10 = "rtsp://mikvideo.ru:10191/user=admin1&password=mikvideo&channel=11&stream=0.sdp?real_stream";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HandlerGst handler(rtsp_url_1);
    handler.play();

    return a.exec();
}
