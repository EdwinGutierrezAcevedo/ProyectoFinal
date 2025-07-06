#include "personaje.h"

Personaje::Personaje(QGraphicsItem *parent)
    : GameObject(parent) {
    // Inicialización común para todos los personajes
}

void Personaje::actualizarLogica() {
    // Lógica común de movimiento/gravedad
    velocidad.setY(velocidad.y() + GRAVEDAD);
    setPos(pos() + velocidad);
}

void Personaje::actualizarGraficos() {
    // Actualización común de gráficos
    if (direccion == DERECHA && !framesDerecha.isEmpty()) {
        setPixmap(framesDerecha[currentFrame]);
    }
    else if (!framesIzquierda.isEmpty()) {
        setPixmap(framesIzquierda[currentFrame]);
    }

    // Avanzar frame de animación
    currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
}
