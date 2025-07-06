#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "gameobject.h"
#include "goku.h"

class Enemigo : public GameObject {
    Q_OBJECT
public:
    enum Tipo { SOLDADO, JEFE };

    explicit Enemigo(Tipo tipo, Goku* goku, QGraphicsItem *parent = nullptr);
    void avanzar(int fase) override;
    void manejarColision(GameObject* otro) override;

    void lanzarGranada();

protected:
    Tipo tipoEnemigo;
    Goku* gokuRef;  // Referencia a Goku para apuntar
    qreal tiempoEntreAtaques = 3.0;  // Segundos
    qreal tiempoAcumulado = 0.0;
    int salud = 100;

    virtual void actualizarIA(qreal deltaTime);
};

#endif // ENEMIGO_H


//Weight= 54, hight= 50
