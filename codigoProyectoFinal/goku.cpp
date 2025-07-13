#include "goku.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include "enemigo.h"
#include <QGraphicsScene>
#include <QAbstractAnimation>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
Goku::Goku(QGraphicsItem *parent)
    : Personaje(parent) {
    // Configuración específica de Goku
    GRAVEDAD = 0.5;
    VELOCIDAD_CAMINAR = 5;
    FUERZA_SALTO = -10;
    FRAME_WIDTH = 37;
    FRAME_HEIGHT = 50;
    TOTAL_FRAMES = 8;
    TOTAL_ATTACK_FRAMES = 4;
    TOTAL_IDLE_FRAMES = 4;

    // Cargar sprite sheet
    spriteSheet = QPixmap(":/img/Goku/GokuCorriendo.png");
    // Cargar sprites de ataque
    attackSpriteSheet = QPixmap(":/img/Goku/GokuAtacando.png");
    // Cargar sprites de IDLE
    idleSpriteSheet = QPixmap(":/img/Goku/GokuIdle.png");

    if(spriteSheet.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite sheet de Goku";
        spriteSheet = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        spriteSheet.fill(Qt::red);
    }
    if(attackSpriteSheet.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite sheet de ataque de Goku";
        attackSpriteSheet = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        attackSpriteSheet.fill(Qt::green);
    }

    //cargarSprites();
    cargarSpritesAtaque();
    cargarSpritesCaminata() ;
    cargarSpritesIdle();
    setPixmap(framesDerecha.first());
    setFlag(QGraphicsItem::ItemIsFocusable);

    // Temporizador para animación
    QTimer* animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, [this]() {
        // Avanzar animación solo para caminar e idle
        if(estado == WALKING || estado == IDLE) {
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
            actualizarGraficos();
        }
    });
    animTimer->start(80); // 80ms = 12.5 FPS
    iniciarAnimacionIdle();

    m_timerGolpe = new QTimer(this);
    m_timerGolpe->setSingleShot(true);
    connect(m_timerGolpe, &QTimer::timeout, this, &Goku::volverASpriteNormal);
}

void Goku::cargarSpritesCaminata() {
    framesDerecha.clear();
    framesIzquierda.clear();

    QPixmap spriteSheet = QPixmap(":/img/Goku/GokuCorriendo.png");
    if(spriteSheet.isNull()) {
        qDebug() << "Error: No se pudo cargar sprite sheet de caminata de Goku";
        spriteSheet = QPixmap(FRAME_WIDTH, FRAME_HEIGHT);
        spriteSheet.fill(Qt::red);
    }

    for(int i = 0; i < TOTAL_FRAMES; i++) {
        int frameX = i * FRAME_WIDTH;
        QPixmap frame = spriteSheet.copy(frameX, 0, FRAME_WIDTH, FRAME_HEIGHT);
        framesDerecha.append(frame);
        framesIzquierda.append(frame.transformed(QTransform().scale(-1, 1)));
    }
}

/*
void Goku::cargarSprites() {
 qDebug() << "cargarSprites() llamado - usar cargarSpritesCaminata() en su lugar";
}
*/
void Goku::cargarSpritesAtaque() {
    attackFramesDerecha.clear();
    attackFramesIzquierda.clear();

    for(int i = 0; i < TOTAL_ATTACK_FRAMES; i++) {
        int frameX = i * FRAME_WIDTH;
        QPixmap frame = attackSpriteSheet.copy(frameX, 0, FRAME_WIDTH, FRAME_HEIGHT);
        attackFramesDerecha.append(frame);
        attackFramesIzquierda.append(frame.transformed(QTransform().scale(-1, 1)));
    }

}

void Goku::cargarSpritesIdle() {
    idleFramesDerecha.clear();
    idleFramesIzquierda.clear();

    for(int i = 0; i < TOTAL_IDLE_FRAMES; i++) {
        int frameX = i * FRAME_WIDTH;
        QPixmap frame = idleSpriteSheet.copy(frameX, 0, FRAME_WIDTH, FRAME_HEIGHT);
        idleFramesDerecha.append(frame);
        idleFramesIzquierda.append(frame.transformed(QTransform().scale(-1, 1)));
    }
    qDebug() << "cargarSprites() llamado - usar cargarSpritesCaminata() en su lugar";
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
    if(y() >= 90) { // Ajustado a la nueva escena
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

    if(velocidad.x() == 0 && estado == WALKING) {
        estado = IDLE;
        idleFrame = 0; // Reiniciar animación idle
        actualizarGraficos();
    }

    if(estado == JUMPING && velocidad.x() == 0) {
        estado = IDLE;
    }
}

void Goku::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Left:
        if(!isAttacking) {
            velocidad.setX(-VELOCIDAD_CAMINAR);
            estado = WALKING;
            if(direccion == DERECHA) {
                direccion = IZQUIERDA;
                actualizarGraficos();
            }
        }
        break;
    case Qt::Key_Right:
        if(!isAttacking) {
            velocidad.setX(VELOCIDAD_CAMINAR);
            estado = WALKING;
            if(direccion == IZQUIERDA) {
                direccion = DERECHA;
                actualizarGraficos();
            }
        }
        break;
    case Qt::Key_Space:
        if(enSuelo && !isAttacking) {
            velocidad.setY(FUERZA_SALTO);
            enSuelo = false;
            estado = JUMPING;
        }
        break;
    case Qt::Key_A:
        if(!isAttacking) {
            iniciarAtaque();
            // Forzar actualización inmediata
            actualizarGraficos();
        }
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
    }
}

void Goku::manejarColision(GameObject* otro) {
    // Implementar lógica de colisión específica de Goku
    // (Ej: recibir daño, recolectar objetos, etc)
}
void Goku::iniciarAtaque() {
    // Primero llama a la implementación base
    Personaje::iniciarAtaque();

    if(scene() && isAttacking) {
        // Calcular área de ataque (delante de Goku)
        QRectF areaAtaque;
        if(direccion == DERECHA) {
            areaAtaque = QRectF(x() + FRAME_WIDTH, y(), FRAME_WIDTH, FRAME_HEIGHT);
        } else {
            areaAtaque = QRectF(x() - FRAME_WIDTH, y(), FRAME_WIDTH, FRAME_HEIGHT);
        }

        // Verificar colisiones con enemigos
        QList<QGraphicsItem*> itemsColisionados = scene()->items(areaAtaque);
        for(QGraphicsItem* item : itemsColisionados) {
            if(Enemigo* enemigo = dynamic_cast<Enemigo*>(item)) {
                enemigo->recibirDano(danoAtaque);
            }
        }
    }
}
void Goku::recibirDano(qreal dano) {
    if (m_muriendo) return;

    salud -= dano;

    if (salud > 0) {
        // Guardar sprite actual
        m_spriteNormal = pixmap();

        // Cambiar a sprite golpeado
        setPixmap(QPixmap(":/img/Goku/GokuGolpeado.png").scaled(FRAME_WIDTH, FRAME_HEIGHT));

        // Calcular retroceso
        m_recoilOffset = QPointF((direccion == DERECHA) ? -2.5 : 1, 0);
        setPos(pos() + m_recoilOffset);

        // Parpadear
        QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect();
        opacityEffect->setOpacity(0.5);
        setGraphicsEffect(opacityEffect);

        // Temporizador para restaurar
        QTimer::singleShot(300, this, [this]() {
            if (salud > 0 && !m_muriendo) {
                setGraphicsEffect(nullptr);
                setPixmap(m_spriteNormal);
                setPos(pos() - m_recoilOffset);  // Volver a posición original
                m_golpeado = false;
            }
        });
    } else {
        // Manejar muerte
        m_muriendo = true;
        setPixmap(QPixmap(":/img/Goku/GokuMuerte.png").scaled(FRAME_WIDTH, FRAME_HEIGHT));

        // Deshabilitar físicas y controles
        velocidad = QPointF(0, 0);
        setFlag(QGraphicsItem::ItemIsFocusable, false);

        // Animación de caída
        QPropertyAnimation* fallAnimation = new QPropertyAnimation(this, "pos");
        fallAnimation->setDuration(1000);
        fallAnimation->setEndValue(QPointF(x(), scene()->height()));
        fallAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        // Eliminar después de animación
        connect(fallAnimation, &QPropertyAnimation::finished, this, [this]() {
            scene()->removeItem(this);
            deleteLater();
            //emit gameOver();
        });
    }
}
void Goku::volverASpriteNormal() {
    if (m_golpeado && salud > 0) {
        setPixmap(m_spriteNormal);
        setGraphicsEffect(nullptr);  // Eliminar efecto de opacidad
        m_golpeado = false;
    }
}
