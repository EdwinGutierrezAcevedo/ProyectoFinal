#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include "goku.h"

class Enemigo : public Personaje {
    Q_OBJECT
public:
    enum Tipo { SOLDADO, JEFE };

    explicit Enemigo(Tipo tipo, Goku* goku, QGraphicsItem *parent = nullptr);
    void avanzar(int fase) override;
    void manejarColision(GameObject* otro) override;
    void lanzarGranada();

protected:
    virtual void actualizarIA(qreal deltaTime);

private:
    void cargarSprites() override;

    Tipo tipoEnemigo;
    Goku* gokuRef;
    qreal tiempoEntreAtaques = 3.0;
    qreal tiempoAcumulado = 0.0;
    int salud = 100;
};

#endif // ENEMIGO_H
