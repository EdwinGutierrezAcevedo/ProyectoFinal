#include "enemigo.h"
//#include "granada.h"
#include <QTimer>
#include <QtMath>
#include <QRandomGenerator>

Enemigo::Enemigo(Tipo tipo, Goku* goku, QGraphicsItem *parent)
    : GameObject(parent), tipoEnemigo(tipo), gokuRef(goku) {

    // Configurar sprite según tipo
    switch(tipo) {
    case SOLDADO:
        setPixmap(QPixmap(":/img/Soldado/SoldadoCorriendo1.png"));
        salud = 80;
        break;
    case JEFE:
        setPixmap(QPixmap(":/img/Soldado/SoldadoCorriendo1.png"));
        salud = 200;
        tiempoEntreAtaques = 1.5;
        break;
    }

    // Temporizador de comportamiento
    QTimer* iaTimer = new QTimer(this);
    connect(iaTimer, &QTimer::timeout, this,[this]() {
        actualizarIA(0.1);  // Actualizar IA cada 100ms
    });
    iaTimer->start(100);
}

void Enemigo::avanzar(int fase) {
    if (fase == 0) {
        // La lógica principal se maneja en actualizarIA
        actualizarIA(0.1);
    }
}

void Enemigo::actualizarIA(qreal deltaTime) {
    if (!gokuRef || !scene()) return;

    tiempoAcumulado += deltaTime;

    /*
    // Lanzar granada cuando sea el momento
    if (tiempoAcumulado >= tiempoEntreAtaques) {
        lanzarGranada();
        tiempoAcumulado = 0;
    }
    */
    // Movimiento básico (perseguir a Goku)
    // Calcular dirección solo en el eje X
    qreal dx = gokuRef->x() - x();  // Diferencia en X
    qreal distanciaX = qAbs(dx);    // Distancia absoluta en X

    if (distanciaX > 100) {  // Mantener distancia mínima en X
        qreal direccionX = (dx > 0) ? 1.0 : -1.0;  // Direccion simplificada
        setPos(x() + direccionX * 1.5, y());       // Mover solo en X
    }
}

/*
void Enemigo::actualizarIA(qreal deltaTime) {
    if (!gokuRef || !scene()) return;

    tiempoAcumulado += deltaTime;
    // Lanzar granada cuando sea el momento
    if (tiempoAcumulado >= tiempoEntreAtaques) {
        lanzarGranada();
        tiempoAcumulado = 0;
    }

    // Movimiento básico (perseguir a Goku)
    QPointF direccion = gokuRef->pos() - pos();
    qreal distancia = QLineF(pos(), gokuRef->pos()).length();

    if (distancia > 100) {  // Mantener distancia mínima
        direccion /= distancia; // Normalizar
        setPos(pos() + direccion * 1.5);
    }
}
*/
/*void Enemigo::lanzarGranada() {
    if (!gokuRef || !scene()) return;

    Granada* granada = new Granada(pos(), gokuRef->pos());
    scene()->addItem(granada);

    // Sonido de lanzamiento
    // emit sonidoLanzamiento("lanzamiento_granada.wav");
}
*/
void Enemigo::manejarColision(GameObject* otro) {
    // Implementar lógica de daño
    salud -= 20;
    if (salud <= 0) {
        // Animación de muerte
        //scene()->removeItem(this);
        deleteLater();
    }
}
