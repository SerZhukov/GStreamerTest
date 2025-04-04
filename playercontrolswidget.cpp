#include "playercontrolswidget.h"

PlayerControlsWidget::PlayerControlsWidget(QWidget *parent)
    : QWidget{parent}
{
    m_layout = new QHBoxLayout;
    setLayout(m_layout);
    m_playButton = new QPushButton("Play");
    m_pauseButton = new QPushButton("Pause");
    m_layout->addWidget(m_playButton);
    m_layout->addWidget(m_pauseButton);
    connect(m_playButton, &QPushButton::clicked, this, [this]()
            {
        emit play_video();
    });
    connect(m_pauseButton, &QPushButton::clicked, this, [this]()
            {
                emit pause_video();
            });

}
