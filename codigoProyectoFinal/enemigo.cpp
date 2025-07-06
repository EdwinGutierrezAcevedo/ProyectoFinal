#include "enemigo.h"
#include <QTimer>
#include <QtMath>
#include <QRandomGenerator>

Enemigo::Enemigo(Tipo tipo, Goku* goku, QGraphicsItem *parent)
    : Personaje(parent), tipoEnemigo(tipo), gokuRef(goku) {

    // Configurar parámetros según tipo
    if (tipo == SOLDADO) {
        GRAVEDAD = 0.0;  // Soldados no necesitan gravedad
        VELOCIDAD_CAMINAR = 1.5;
        FRAME_WIDTH = 35;
        FRAME_HEIGHT = 45;
        salud = 80;
    } else { // JEFE
        GRAVEDAD = 0.0;
        VELOCIDAD_CAMINAR = 1.0;
        FRAME_WIDTH = 60;
        FRAME_HEIGHT = 70;
        salud = 200;
        tiempoEntreAtaques = 1.5;
    }

    TOTAL_FRAMES = 1;  // Solo 1 frame por defecto

    cargarSprites();
    setPixmap(framesDerecha.first());

    // Dirección inicial
    direccion = (gokuRef && gokuRef->x() > x()) ? DERECHA : IZQUIERDA;

    // Temporizador de comportamiento
    QTimer* iaTimer = new QTimer(this);
    connect(iaTimer, &QTimer::timeout, this, [this]() {
        actualizarIA(0.1);  // Actualizar IA cada 100ms
    });
    iaTimer->start(100);
}

void Enemigo::cargarSprites() {
    framesDerecha.clear();
    framesIzquierda.clear();

    // Cargar sprite según tipo
    QPixmap baseSprite;
    switch(tipoEnemigo) {
    case SOLDADO:
        baseSprite = QPixmap(":/img/Soldado/SoldadoCorriendo1.png");
        break;
    case JEFE:
        baseSprite = QPixmap(":/img/Soldado/SoldadoCorriendo1.png");
        break;
    }

    if(baseSprite.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite de enemigo";
        baseSprite = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        baseSprite.fill(Qt::blue);
    }

    // Si solo tenemos 1 frame
    framesDerecha.append(baseSprite);
    QPixmap flipped = baseSprite.transformed(QTransform().scale(-1, 1));
    framesIzquierda.append(flipped);
}

void Enemigo::avanzar(int fase) {
    if (fase == 0) {
        // Aplicar lógica base (gravedad, movimiento)
        actualizarLogica();
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
    qreal dx = gokuRef->x() - x();  // Diferencia en X
    qreal distanciaX = qAbs(dx);    // Distancia absoluta en X

    if (distanciaX > 100) {  // Mantener distancia mínima en X
        // Determinar dirección
        direccion = (dx > 0) ? DERECHA : IZQUIERDA;

        // Establecer velocidad
        qreal direccionX = (dx > 0) ? 1.0 : -1.0;
        velocidad.setX(direccionX * VELOCIDAD_CAMINAR);

        // Actualizar gráficos si cambió la dirección
        actualizarGraficos();
    } else {
        // Detenerse si está cerca
        velocidad.setX(0);
    }
}

/*void Enemigo::lanzarGranada() {
    if (!gokuRef || !scene()) return;

    Granada* granada = new Granada(pos(), gokuRef->pos());
    scene()->addItem(granada);
}*/

void Enemigo::manejarColision(GameObject* otro) {
    // Implementar lógica de daño
    salud -= 20;
    if (salud <= 0) {
        // Animación de muerte
        //scene()->removeItem(this);
        deleteLater();
    }
}
