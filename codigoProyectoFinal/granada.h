#ifndef GRANADA_H
#define GRANADA_H

#include "proyectil.h"

class Granada : public Proyectil {
    Q_OBJECT
public:
    Granada(const QVector2D& velocidadInicial, float gravedad = 0.5f, QGraphicsItem* parent = nullptr);

protected:
    void explotar() override;

private:
    QTimer* m_timerExplosion;
};

#endif // GRANADA_H
