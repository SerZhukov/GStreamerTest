#include "screen.h"

Screen::Screen(QWidget *parent)
    : QWidget{parent}
{
    m_layout = new QVBoxLayout;
    setLayout(m_layout);
    m_screen = new QWidget;
    m_screen->resize(400, 400);
    m_controls = new PlayerControlsWidget;
    m_layout->addWidget(m_screen);
    m_layout->addWidget(m_controls);
    connect(m_controls, &PlayerControlsWidget::play_video, this,
            [this](){
                emit play_video();
    });
    connect(m_controls, &PlayerControlsWidget::pause_video, this,
            [this](){
                qDebug() << "emit pause_video();";
                emit pause_video();
            });
}

WId Screen::getWId()
{
    return m_screen->winId();
}
