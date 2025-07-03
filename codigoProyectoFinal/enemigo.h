#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QObject>

class Enemigo : public QObject
{
    Q_OBJECT
public:
    explicit Enemigo(QObject *parent = nullptr);

signals:
};

#endif // ENEMIGO_H
