#ifndef HANDLERGST_H
#define HANDLERGST_H

#include <QObject>

class HandlerGst : public QObject
{
    Q_OBJECT
public:
    explicit HandlerGst(QObject *parent = nullptr);

signals:
};

#endif // HANDLERGST_H
