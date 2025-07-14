#include "granada.h"
#include "explosion.h"
#include "QGraphicsScene"
#include <QTimer>

Granada::Granada(const QVector2D& velocidadInicial, float gravedad, QGraphicsItem* parent)
    : Proyectil(velocidadInicial, gravedad, parent) {

    // Timer para explosión automática después de 3 segundos
    m_timerExplosion = new QTimer(this);
    m_timerExplosion->setSingleShot(true);
    connect(m_timerExplosion, &QTimer::timeout, this, [this]() {
        if (!m_explotado) explotar();
    });
    m_timerExplosion->start(3000);
    if (parent && parent->scene()) {
        parent->scene()->addItem(this);
    }
}

void Granada::explotar() {
    if (m_explotado) return;
    m_explotado = true;

    // Crear explosión
    Explosion* explosion = new Explosion(100, 20); // Radio 100, daño 20
    explosion->setPos(pos());
    scene()->addItem(explosion);

    // Eliminar esta granada
    emit objetoDestruido(this);
    if (scene()) {
        scene()->removeItem(this);
    }
}
