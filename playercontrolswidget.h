#ifndef PLAYERCONTROLSWIDGET_H
#define PLAYERCONTROLSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class PlayerControlsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerControlsWidget(QWidget *parent = nullptr);
private:
    QHBoxLayout* m_layout;
    QPushButton* m_playButton;
    QPushButton* m_pauseButton;
signals:
    void pause_video();
    void play_video();
};

#endif // PLAYERCONTROLSWIDGET_H
