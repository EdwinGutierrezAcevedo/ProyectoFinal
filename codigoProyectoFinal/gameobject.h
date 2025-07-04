#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsPixmapItem>
#include <QObject>

class GameObject : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit GameObject(QGraphicsItem *parent = nullptr);
    virtual void avanzar(int fase) = 0;  // fase 0: lógica, fase 1: gráficos
    virtual void manejarColision(GameObject* otro) {}

signals:
    void objetoDestruido(GameObject* obj);
};

#endif // GAMEOBJECT_H
