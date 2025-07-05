#include "goku.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QPainter>

Goku::Goku(QGraphicsItem *parent) : GameObject(parent) {
    // Cargar sprite sheet completo (240x40 px)
    spriteSheet = QPixmap(":/img/Goku/GokuCorriendo.png");

    // Validar carga
    if(spriteSheet.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite sheet de Goku";
        // Crear placeholder rojo para debug
        spriteSheet = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        spriteSheet.fill(Qt::red);
    }

    // Precargar todos los frames
    cargarSprites();

    // Configurar sprite inicial
    setPixmap(framesDerecha.first());

    // Configurar para recibir eventos de teclado
    setFlag(QGraphicsItem::ItemIsFocusable);

    // Temporizador para animación
    QTimer* animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout,this, [this]() {
        if(estado == WALKING) {
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
            actualizarGraficos();
        }
    });
    animTimer->start(80); // 80ms = 12.5 FPS
}

void Goku::cargarSprites() {
    for(int i = 0; i < TOTAL_FRAMES; i++) {
        int frameX = i * FRAME_WIDTH;

        // Recortar frame del sprite sheet
        QPixmap frame = spriteSheet.copy(frameX, 0, FRAME_WIDTH, FRAME_HEIGHT);

        // Guardar versión derecha e izquierda
        framesDerecha.append(frame);
        framesIzquierda.append(frame.transformed(QTransform().scale(-1, 1)));
    }
}

void Goku::avanzar(int fase) {
    if(fase == 0) { // Fase de lógica
        actualizarLogica();
    }
    else if(fase == 1) { // Fase de gráficos
        actualizarGraficos();
    }
}

void Goku::actualizarLogica() {
    // Aplicar gravedad
    if(!enSuelo) {
        velocidad.setY(velocidad.y() + GRAVEDAD);
    }

    // Actualizar posición
    setPos(x() + velocidad.x(), y() + velocidad.y());

    // Verificar colisión con suelo
    if(y() > 90) { // Ajustado a la nueva escena
        setY(90);
        enSuelo = true;
        velocidad.setY(0);

        // Si estaba saltando, volver a estado anterior
        if(estado == JUMPING) {
            estado = velocidad.x() != 0 ? WALKING : IDLE;
        }
    }

    // Actualizar estado
    if(velocidad.x() == 0 && estado == WALKING) {
        estado = IDLE;
        currentFrame = 0;
        actualizarGraficos();
    }
}

void Goku::actualizarGraficos() {
    // Solo actualizar si tenemos frames válidos
    if(currentFrame < 0 || currentFrame >= framesDerecha.size()) return;

    // Seleccionar frame basado en dirección
    if(mirandoDerecha) {
        setPixmap(framesDerecha[currentFrame]);
    } else {
        setPixmap(framesIzquierda[currentFrame]);
    }
}

void Goku::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Left:
        velocidad.setX(-VELOCIDAD_CAMINAR);
        estado = WALKING;
        if(mirandoDerecha) {
            mirandoDerecha = false;
            actualizarGraficos(); // Actualizar inmediatamente
        }
        break;
    case Qt::Key_Right:
        velocidad.setX(VELOCIDAD_CAMINAR);
        estado = WALKING;
        if(!mirandoDerecha) {
            mirandoDerecha = true;
            actualizarGraficos(); // Actualizar inmediatamente
        }
        break;
    case Qt::Key_Space:
        if(enSuelo) {
            velocidad.setY(FUERZA_SALTO);
            enSuelo = false;
            estado = JUMPING;
        }
        break;
    case Qt::Key_A:
        estado = ATTACKING;
        // Lógica de ataque (implementar después)
        qDebug() << "Goku ataca!";
        break;
    }
}

void Goku::keyReleaseEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Left:
        if(velocidad.x() < 0) velocidad.setX(0);
        break;
    case Qt::Key_Right:
        if(velocidad.x() > 0) velocidad.setX(0);
        break;
    case Qt::Key_A:
        if(estado == ATTACKING) {
            estado = IDLE;
            actualizarGraficos();
        }
        break;
    }
}
