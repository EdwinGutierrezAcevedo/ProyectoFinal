#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "gameobject.h"

class Explosion : public GameObject {
    Q_OBJECT
public:
    Explosion(int radio, int dano, QGraphicsItem* parent = nullptr);

    void avanzar(int fase) override;
    void manejarColision(GameObject* otro) override;

    int radio() const { return m_radio; }
    int dano() const { return m_dano; }

private:
    void expandir();

    int m_radio;
    int m_dano;
    int m_radioActual = 5;
    QTimer* m_timerExpansion;
    QTimer* m_timerDesvanecer;
};

#endif // EXPLOSION_H
