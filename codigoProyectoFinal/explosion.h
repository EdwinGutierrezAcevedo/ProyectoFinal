#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <QObject>

class Explosion : public QObject
{
    Q_OBJECT
public:
    explicit Explosion(QObject *parent = nullptr);

signals:
};

#endif // EXPLOSION_H
