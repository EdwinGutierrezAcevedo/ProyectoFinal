#include "proyectil.h"
#include "goku.h"

Proyectil::Proyectil(const QVector2D& velocidadInicial, float gravedad, QGraphicsItem* parent)
    : GameObject(parent), m_velocidad(velocidadInicial), m_gravedad(gravedad) {
    // Configurar tamaño del proyectil
    setPixmap(QPixmap(":/img/granada.png"));
}

void Proyectil::avanzar(int fase) {
    if (fase == 0 && !m_explotado) {
        // AJUSTE: Aplicar gravedad con un factor más realista
        m_velocidad.setY(m_velocidad.y() + m_gravedad * 0.015); // Aumentado de 0.016 a 0.5

        // AJUSTE: Reducir la velocidad horizontal con el tiempo
        m_velocidad.setX(m_velocidad.x() * 0.98); // Fricción del aire

        // Actualizar posición
        setPos(x() + m_velocidad.x(), y() + m_velocidad.y());

        // Verificar colisión con el suelo (ajustar según tu nivel)
        if (y() > 100) { // Valor ajustado para tu escenario
            explotar();
        }
    }
}

void Proyectil::manejarColision(GameObject* otro) {
    if (dynamic_cast<Goku*>(otro)) {
        explotar();
    }
}
