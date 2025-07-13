#include "personaje.h"
#include "QGraphicsScene"

Personaje::Personaje(QGraphicsItem *parent)
    : GameObject(parent), attackTimer(nullptr) {
}

Personaje::~Personaje() {
    if(attackTimer) delete attackTimer;
}

void Personaje::iniciarAnimacionIdle() {
    if(!idleAnimTimer) {
        idleAnimTimer = new QTimer(this);
        connect(idleAnimTimer, &QTimer::timeout, this, [this]() {
            if(estado == IDLE && !isAttacking) {
                idleFrame = (idleFrame + 1) % TOTAL_IDLE_FRAMES;
                actualizarGraficos();
            }
        });
        idleAnimTimer->start(200); // Cambiar frame cada 200ms
    }
}

void Personaje::actualizarLogica() {
    if(!isAttacking) {
        // Lógica común solo aplica si no está atacando
        velocidad.setY(velocidad.y() + GRAVEDAD);
        setPos(pos() + velocidad);
    }
}

void Personaje::actualizarGraficos() {
    if(isAttacking) {
        // Manejar gráficos de ataque
        if(attackFrame < attackFramesDerecha.size()) {
            if(direccion == DERECHA) {
                setPixmap(attackFramesDerecha[attackFrame]);
            } else {
                setPixmap(attackFramesIzquierda[attackFrame]);
            }
        }
        return;
    }

    // Manejar gráficos de caminata
    if(estado == WALKING && !framesDerecha.isEmpty()) {
        if(direccion == DERECHA) {
            setPixmap(framesDerecha[currentFrame]);
        } else {
            setPixmap(framesIzquierda[currentFrame]);
        }
        return;
    }

    if(estado == IDLE && !idleFramesDerecha.isEmpty()) {
        if(direccion == DERECHA) {
            setPixmap(idleFramesDerecha[idleFrame]);
        } else {
            setPixmap(idleFramesIzquierda[idleFrame]);
        }
        return;
    }

    // Gráficos normales (base)
    if (direccion == DERECHA && !framesDerecha.isEmpty()) {
        setPixmap(framesDerecha[currentFrame]);
    }
    else if (!framesIzquierda.isEmpty()) {
        setPixmap(framesIzquierda[currentFrame]);
    }
}

void Personaje::iniciarAtaque() {
        if(!isAttacking && enSuelo) {
            isAttacking = true;
            attackFrame = 0;
            estado = ATTACKING;
            velocidad.setX(0);  // Detener movimiento durante ataque

            if(!attackTimer) {
                attackTimer = new QTimer(this);
                attackTimer->setSingleShot(true);
                connect(attackTimer, &QTimer::timeout, this, [this]() {
                    isAttacking = false;
                    attackFrame = 0;
                    estado = IDLE;
                    actualizarGraficos();
                    emit ataqueTerminado();
                });
            }

            QTimer* frameTimer = new QTimer(this);
            frameTimer->setSingleShot(false);
            connect(frameTimer, &QTimer::timeout, this, [this, frameTimer]() {
                if(attackFrame < TOTAL_ATTACK_FRAMES - 1) {
                    attackFrame++;
                    actualizarGraficos();
                } else {
                    frameTimer->stop();
                    frameTimer->deleteLater();
                }
            });
            frameTimer->start(100);

            attackTimer->start(TOTAL_ATTACK_FRAMES * 100);
        }
        // REMOVER EL CÓDIGO DE DETECCIÓN DE COLISIONES DE AQUÍ
    }

        // Verificar colisiones con enemigos
        /*QList<QGraphicsItem*> itemsColisionados = scene()->items(areaAtaque);
        for(QGraphicsItem* item : itemsColisionados) {
            if(Enemigo* enemigo = dynamic_cast<Enemigo*>(item)) {
                enemigo->recibirDano(danoAtaque);
            }*/






