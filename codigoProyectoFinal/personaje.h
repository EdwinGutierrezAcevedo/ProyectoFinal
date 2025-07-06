#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "gameobject.h"
#include <QPixmap>
#include <QVector>

class Personaje : public GameObject {
    Q_OBJECT
public:
    enum State { IDLE, WALKING, JUMPING, ATTACKING };
    enum Direction { DERECHA, IZQUIERDA };

    explicit Personaje(QGraphicsItem *parent = nullptr);

    // Métodos comunes
    void avanzar(int fase) override = 0;  // Virtual puro
    void manejarColision(GameObject* otro) override = 0;  // Virtual puro
    virtual void actualizarLogica();
    virtual void actualizarGraficos();
    virtual void cargarSprites() = 0;  // Virtual puro

protected:
    State estado = IDLE;
    Direction direccion = DERECHA;
    bool enSuelo = true;
    QPointF velocidad;

    // Parámetros físicos
    qreal GRAVEDAD = 0.5;
    qreal VELOCIDAD_CAMINAR = 5;
    qreal FUERZA_SALTO = -10;

    // Sistema de sprite sheet
    QPixmap spriteSheet;
    int FRAME_WIDTH = 0;
    int FRAME_HEIGHT = 0;
    int currentFrame = 0;
    int TOTAL_FRAMES = 0;
    QVector<QPixmap> framesDerecha;
    QVector<QPixmap> framesIzquierda;
};

#endif // PERSONAJE_H
