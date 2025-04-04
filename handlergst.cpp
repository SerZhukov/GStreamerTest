#include "handlergst.h"

HandlerGst::HandlerGst(const QString& rtspLink, QObject *parent)
    : QObject{parent}, m_rtspLink(rtspLink)
{
    m_screen = new Screen;
    m_screen->resize(400, 400);
    m_screen->show();
    m_wid = m_screen->getWId();
    m_player = new Player;
    m_player->setRtsp(m_rtspLink);
    m_player->setWinId(m_wid);
    m_player->moveToThread(&workerThread);
    connect(this, &HandlerGst::playSignal, m_player, &Player::play_1);
    connect(m_screen, &Screen::pause_video, m_player, &Player::pause_video,Qt::QueuedConnection);
    connect(m_screen, &Screen::play_video, m_player, &Player::play_video, Qt::QueuedConnection);
    workerThread.start();
    qDebug() << "Main thread: " << QThread::currentThreadId();
}

HandlerGst::~HandlerGst()
{
    delete m_screen;
}

void HandlerGst::play()
{

    emit playSignal();
    qDebug() << "emit playSignal();";
}
