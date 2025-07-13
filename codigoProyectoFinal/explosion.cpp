#include "explosion.h"
#include "goku.h"
#include <QPainter>
#include <QTimer>
#include <QGraphicsScene>

Explosion::Explosion(int radio, int dano, QGraphicsItem* parent)
    : GameObject(parent), m_radio(radio), m_dano(dano) {
    // Configurar apariencia inicial
    setPixmap(QPixmap(":/img/explosion.png").scaled(20, 20));
    setZValue(10); // Asegurarse que esté encima de otros objetos

    // Timer para expansión
    m_timerExpansion = new QTimer(this);
    connect(m_timerExpansion, &QTimer::timeout, this, &Explosion::expandir);
    m_timerExpansion->start(50); // Expandir cada 50ms

    // Timer para desvanecer
    m_timerDesvanecer = new QTimer(this);
    connect(m_timerDesvanecer, &QTimer::timeout,this, [this]() {
        //mit objetoDestruido(this);
        if (scene()) {
            scene()->removeItem(this);
        }
        deleteLater();
    });
    m_timerDesvanecer->start(500); // Desaparecer después de 500ms
}

void Explosion::expandir() {
    m_radioActual += 15;
    if (m_radioActual > m_radio) {
        m_timerExpansion->stop();
        return;
    }

    // Actualizar tamaño visual
    setPixmap(QPixmap(":/img/explosion.png").scaled(m_radioActual * 2, m_radioActual * 2));
    setOffset(-m_radioActual, -m_radioActual); // Centrar

    // Verificar colisiones con objetos en la escena
    for (QGraphicsItem* item : scene()->items()) {
        if (auto gameObj = dynamic_cast<GameObject*>(item)) {
            if (gameObj != this && collidesWithItem(gameObj)) {
                manejarColision(gameObj);
            }
        }
    }
}

void Explosion::manejarColision(GameObject* otro) {
    if (auto goku = dynamic_cast<Goku*>(otro)) {
        qreal distancia = QLineF(pos(), goku->pos()).length();
        if (distancia <= m_radioActual) {
            qreal factor = 1.0 - (distancia / m_radioActual);
            goku->recibirDano(m_dano * factor);  // Descomentado y corregido
        }
    }
}

void Explosion::avanzar(int fase) {
    // No se necesita lógica adicional en cada frame
}

