
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

private:
    void actualizarLogica() override;
    void cargarSprites() override;
    void cargarSpritesAtaque() override;
    void cargarSpritesIdle() override;
};

#endif // GOKU_H


