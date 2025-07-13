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
    void recibirDano(qreal dano)  ;

protected:
    virtual void actualizarIA(qreal deltaTime);
    //void cargarSprites() override;
    void cargarSpritesAtaque() override;
    void cargarSpritesIdle() override;
    void cargarSpritesCaminata() override;
private:
    Tipo tipoEnemigo;
    Goku* gokuRef;
    qreal tiempoEntreAtaques = 3.0;
    qreal tiempoAcumulado = 0.0;
    int salud = 100;
    QTimer* walkAnimTimer = nullptr;
    Tipo m_tipo;
    qreal m_salud;
    bool m_golpeado;  // Estado de golpeado
    QTimer* m_timerGolpe;
    //void lanzarGranada();
private slots:
    void lanzarGranadaDespuesDeAtaque();
    void volverASpriteNormal();
signals:
    void eliminado();
};

#endif // ENEMIGO_H
