#ifndef HANDLERGST_H
#define HANDLERGST_H

#include <QObject>
#include "screen.h"
#include "player.h"

class HandlerGst : public QObject
{
    Q_OBJECT
public:
    explicit HandlerGst(const QString& rtspLink, QObject *parent = nullptr);
    ~HandlerGst();
public slots:
    void play();
private:
    QString m_rtspLink;
    Screen* m_screen;
    Player* m_player;
    WId m_wid;

signals:
    void playSignal();
};

#endif // HANDLERGST_H
