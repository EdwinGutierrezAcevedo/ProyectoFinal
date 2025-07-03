#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>

class Personaje : public QObject
{
    Q_OBJECT
public:
    explicit Personaje(QObject *parent = nullptr);

signals:
};

#endif // PERSONAJE_H
