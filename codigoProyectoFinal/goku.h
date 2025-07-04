#ifndef GOKU_H
#define GOKU_H

#include "gameobject.h"

class Goku : public GameObject {
    Q_OBJECT
public:
    explicit Goku(QGraphicsItem *parent = nullptr);

    void avanzar(int fase) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    enum State { IDLE, WALKING, JUMPING, ATTACKING };

private:
    State estado = IDLE;
    bool mirandoDerecha = true;
    bool enSuelo = true;
    QPointF velocidad;

    // Parámetros físicos
    const qreal GRAVEDAD = 0.5;
    const qreal VELOCIDAD_CAMINAR = 5;
    const qreal FUERZA_SALTO = -10;

    // Sistema de sprite sheet (240x40 px, 8 frames)
    QPixmap spriteSheet;
    const int FRAME_WIDTH = 38;   // 240px / 8 frames = 30px
    const int FRAME_HEIGHT = 50;  // Altura del sprite
    int currentFrame = 0;
    const int TOTAL_FRAMES = 8;
    QVector<QPixmap> framesDerecha;
    QVector<QPixmap> framesIzquierda;

    void actualizarLogica();
    void actualizarGraficos();
    void cargarSprites();
};

#endif // GOKU_H
