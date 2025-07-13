
#ifndef GOKU_H
#define GOKU_H

#include "personaje.h"

class Goku : public Personaje {
    Q_OBJECT
public:
    explicit Goku(QGraphicsItem *parent = nullptr);

    void avanzar(int fase) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void manejarColision(GameObject* otro) override;
    void recibirDano(qreal dano);
    void iniciarAtaque() override;
private:
    void actualizarLogica() override;
    //void cargarSprites() override;
    void cargarSpritesAtaque() override;
    void cargarSpritesIdle() override;
    void cargarSpritesCaminata() override;

    bool m_golpeado = false;
    QTimer* m_timerGolpe = nullptr;
    QPixmap m_spriteNormal;

    QPointF m_recoilOffset;  // Almacenar desplazamiento de retroceso

    // Nuevo estado de muerte
    bool m_muriendo = false;
private slots:
    void volverASpriteNormal();
};

#endif // GOKU_H


