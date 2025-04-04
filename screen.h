#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include "playercontrolswidget.h"
#include <QVBoxLayout>
class Screen : public QWidget
{
    Q_OBJECT
public:
    explicit Screen(QWidget *parent = nullptr);
public slots:
    WId getWId();
private:
    QVBoxLayout* m_layout;
    QWidget* m_screen;
    PlayerControlsWidget* m_controls;
signals:
    void play_video();
    void pause_video();

};

#endif // SCREEN_H
