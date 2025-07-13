#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "gameobject.h"
#include <QPixmap>
#include <QVector>
#include <QTimer>

class Personaje : public GameObject {
    Q_OBJECT
public:
    enum State { IDLE, WALKING, JUMPING, ATTACKING };
    enum Direction { DERECHA, IZQUIERDA };

    explicit Personaje(QGraphicsItem *parent = nullptr);
    virtual ~Personaje();

    // Métodos comunes
    void avanzar(int fase) override = 0;
    void manejarColision(GameObject* otro) override = 0;
    virtual void actualizarLogica();
    virtual void actualizarGraficos();
    //virtual void cargarSprites() = 0;

    // Nuevos métodos para ataque
    virtual void iniciarAtaque();
    virtual void cargarSpritesAtaque() = 0;  // Obligatorio para personajes atacantes
    virtual void cargarSpritesIdle() = 0;
    virtual void cargarSpritesCaminata()=0;
    virtual void iniciarAnimacionIdle();


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
    QPixmap attackSpriteSheet;  // Nueva hoja de sprites para ataque
    QPixmap idleSpriteSheet;
    int FRAME_WIDTH = 0;
    int FRAME_HEIGHT = 0;
    int currentFrame = 0;
    int TOTAL_FRAMES = 0;
    QVector<QPixmap> framesDerecha;
    QVector<QPixmap> framesIzquierda;

    // Para ataque
    QVector<QPixmap> attackFramesDerecha;
    QVector<QPixmap> attackFramesIzquierda;
    int attackFrame = 0;
    int TOTAL_ATTACK_FRAMES = 0;
    bool isAttacking = false;
    QTimer* attackTimer = nullptr;

    //Para IDLE
    QVector<QPixmap> idleFramesDerecha;
    QVector<QPixmap> idleFramesIzquierda;
    int idleFrame = 0;
    int TOTAL_IDLE_FRAMES = 0;
    QTimer* idleAnimTimer = nullptr;

signals:
    void ataqueTerminado();

};

#endif // PERSONAJE_H

