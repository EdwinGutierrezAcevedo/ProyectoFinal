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
        FRAME_WIDTH = 53;
        FRAME_HEIGHT = 55;
        salud = 80;
        TOTAL_IDLE_FRAMES = 4;
        TOTAL_FRAMES = 8;
    } else { // JEFE
        GRAVEDAD = 0.0;
        VELOCIDAD_CAMINAR = 1.0;
        FRAME_WIDTH = 53;
        FRAME_HEIGHT = 55;
        salud = 200;
        tiempoEntreAtaques = 1.5;
        TOTAL_IDLE_FRAMES = 4;
        TOTAL_FRAMES = 8;
    }

    cargarSprites();
    cargarSpritesAtaque();
    cargarSpritesIdle();
    cargarSpritesCaminata();
    setPixmap(framesDerecha.first());

    // Dirección inicial
    direccion = (gokuRef && gokuRef->x() > x()) ? DERECHA : IZQUIERDA;

    // Temporizador de comportamiento
    QTimer* iaTimer = new QTimer(this);
    connect(iaTimer, &QTimer::timeout, this, [this]() {
        actualizarIA(0.1);  // Actualizar IA cada 100ms
    });
    iaTimer->start(100);
    // Temporizador para animación de caminata (solo soldados)
    if (tipoEnemigo == SOLDADO) {
        walkAnimTimer = new QTimer(this);
        connect(walkAnimTimer, &QTimer::timeout, this, [this]() {
            if (estado == WALKING) {
                currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
                actualizarGraficos();
            }
        });
        walkAnimTimer->start(100); // Velocidad de animación: 10 FPS
    }

    // Iniciar animación idle
    iniciarAnimacionIdle();
}

void Enemigo::cargarSprites() {
qDebug() << "cargarSprites() llamado - usar cargarSpritesCaminata() en su lugar";
}

void Enemigo::cargarSpritesCaminata() {
    framesDerecha.clear();
    framesIzquierda.clear();

    QPixmap baseSprite;
    switch(tipoEnemigo) {
    case SOLDADO:
        baseSprite = QPixmap(":/img/Soldado/SoldadoCorriendo.png"); // Nueva imagen con 8 sprites
        break;
    case JEFE:
        baseSprite = QPixmap(":/img/Soldado/SoldadoCorriendo.png");
        break;
    }

    if(baseSprite.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite de caminata de enemigo";
        baseSprite = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        baseSprite.fill(Qt::blue);
    }

    // Cargar todos los frames de caminata
    for(int i = 0; i < TOTAL_FRAMES; i++) {
        int frameX = i * FRAME_WIDTH;
        QPixmap frame = baseSprite.copy(frameX, 0, FRAME_WIDTH, FRAME_HEIGHT);
        framesDerecha.append(frame);
        framesIzquierda.append(frame.transformed(QTransform().scale(-1, 1)));
    }
}

void Enemigo::cargarSpritesAtaque() {
    attackFramesDerecha.clear();
    attackFramesIzquierda.clear();

    QPixmap attackSprite;
    switch(tipoEnemigo) {
    case SOLDADO:
        attackSprite = QPixmap(":/img/Soldado/SoldadoAtacando.png");
        TOTAL_ATTACK_FRAMES = 6;
        break;
    case JEFE:
        attackSprite = QPixmap(":/img/Soldado/SoldadoAtacando.png");
        TOTAL_ATTACK_FRAMES = 6;
        break;
    }

    if(attackSprite.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite de ataque de enemigo";
        attackSprite = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        attackSprite.fill(Qt::green);
    }

    for(int i = 0; i < TOTAL_ATTACK_FRAMES; i++) {
        int frameX = i * FRAME_WIDTH;
        QPixmap frame = attackSprite.copy(frameX, 0, FRAME_WIDTH, FRAME_HEIGHT);
        attackFramesDerecha.append(frame);
        attackFramesIzquierda.append(frame.transformed(QTransform().scale(-1, 1)));
    }
}

void Enemigo::cargarSpritesIdle() {
    idleFramesDerecha.clear();
    idleFramesIzquierda.clear();

    QPixmap idleSprite;
    switch(tipoEnemigo) {
    case SOLDADO:
        idleSprite = QPixmap(":/img/Soldado/SoldierIdle.png");
        break;
    case JEFE:
        idleSprite = QPixmap(":/img/Soldado/SoldierIdle.png");
        break;
    }

    if(idleSprite.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite idle de enemigo";
        idleSprite = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        idleSprite.fill(Qt::yellow);
    }

    for(int i = 0; i < TOTAL_IDLE_FRAMES; i++) {
        int frameX = i * FRAME_WIDTH;
        QPixmap frame = idleSprite.copy(frameX, 0, FRAME_WIDTH, FRAME_HEIGHT);
        idleFramesDerecha.append(frame);
        idleFramesIzquierda.append(frame.transformed(QTransform().scale(-1, 1)));
    }
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

        estado=WALKING;

        // Actualizar gráficos si cambió la dirección
        actualizarGraficos();
    } else {
        // Detenerse si está cerca
        velocidad.setX(0);
        if(tiempoAcumulado >= tiempoEntreAtaques) {
            iniciarAtaque();
            tiempoAcumulado = 0;
        }else {
            // Detenerse si está cerca
            velocidad.setX(0);
            estado = IDLE; // Cambiar a estado IDLE
        }
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





