#ifndef PROYECTIL_H
#define PROYECTIL_H

#include "gameobject.h"
#include <QVector2D>

class Proyectil : public GameObject {
    Q_OBJECT
public:
    Proyectil(const QVector2D& velocidadInicial, float gravedad, QGraphicsItem* parent = nullptr);

    void avanzar(int fase) override;
    void manejarColision(GameObject* otro) override;

    QVector2D velocidad() const { return m_velocidad; }
    void setVelocidad(const QVector2D& nuevaVelocidad) { m_velocidad = nuevaVelocidad; }

protected:
    virtual void explotar() = 0; // Método abstracto para explosión

    QVector2D m_velocidad;
    float m_gravedad;
    bool m_explotado = false;
};

#endif // PROYECTIL_H
