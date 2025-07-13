#include "nivel.h"
#include <QTimer>

Nivel::Nivel(Nivel::NumeroNivel numero, Goku* goku, QGraphicsScene* scene, QObject* parent)
    : QObject(parent), m_numero(numero), m_goku(goku), m_scene(scene),
    m_completado(false), m_verificador(nullptr) {}

Nivel::~Nivel() {
    limpiar();
    if (m_verificador) {
        m_verificador->stop();
        m_verificador->deleteLater();
    }
}

void Nivel::iniciar() {
    // Configurar fondo según el nivel
    QString fondo = (m_numero == NIVEL1) ? ":/img/Base.gif" : ":/img/Base.gif";
    m_scene->setBackgroundBrush(QBrush(QPixmap(fondo)));

    // Crear enemigos
    crearEnemigos();

    // Temporizador para verificar si el nivel está completado
    if (!m_verificador) {
        m_verificador = new QTimer(this);
        connect(m_verificador, &QTimer::timeout, this, &Nivel::verificarCompletado);
    }
    m_verificador->start(500); // Verificar cada 500ms
}

void Nivel::crearEnemigos() {
    if (m_numero == NIVEL1) {
        Enemigo* soldado = new Enemigo(Enemigo::SOLDADO, m_goku);
        // Corregido: usar 'soldado' en lugar de 'enemigo'
        connect(soldado, &Enemigo::eliminado, this, [this]() {
            emit enemigoEliminado();
        });
        soldado->setPos(390, 90);
        m_scene->addItem(soldado);
        m_enemigos.append(soldado);
    }
    else if (m_numero == NIVEL2) {
        Enemigo* soldado1 = new Enemigo(Enemigo::SOLDADO, m_goku);
        // Corregido: usar 'soldado1'
        connect(soldado1, &Enemigo::eliminado, this, [this]() {
            emit enemigoEliminado();
        });
        soldado1->setPos(300, 90);
        m_scene->addItem(soldado1);
        m_enemigos.append(soldado1);

        /*Enemigo* soldado2 = new Enemigo(Enemigo::SOLDADO, m_goku);
        // Corregido: usar 'soldado2'
        connect(soldado2, &Enemigo::eliminado, this, [this]() {
            emit enemigoEliminado();
        });
        soldado2->setPos(500, 90);
        m_scene->addItem(soldado2);
        m_enemigos.append(soldado2);

        Enemigo* jefe = new Enemigo(Enemigo::JEFE, m_goku);
        // Corregido: usar 'jefe'
        connect(jefe, &Enemigo::eliminado, this, [this]() {
            emit enemigoEliminado();
        });
        jefe->setPos(700, 90);
        m_scene->addItem(jefe);
        m_enemigos.append(jefe);*/
    }
}

void Nivel::verificarCompletado() {
    // Eliminar enemigos destruidos de la lista
    auto it = m_enemigos.begin();
    while (it != m_enemigos.end()) {
        if (*it == nullptr || !m_scene->items().contains(*it)) {
            it = m_enemigos.erase(it);
        } else {
            ++it;
        }
    }

    // Completado si no quedan enemigos
    if (m_enemigos.isEmpty() && !m_completado) {
        m_completado = true;
        emit completado();
    }
    if (m_enemigos.isEmpty() && !m_completado) {
        m_completado = true;
        emit completado();
    }

}

void Nivel::limpiar() {
    // Eliminar enemigos de la escena y memoria
    for (Enemigo* enemigo : m_enemigos) {
        if (enemigo) {
            if (m_scene->items().contains(enemigo)) {
                m_scene->removeItem(enemigo);
            }
            delete enemigo;
        }
    }
    m_enemigos.clear();
}

bool Nivel::estaCompletado() const {
    return m_completado;
}

Nivel::NumeroNivel Nivel::numero() const {
    return m_numero;
}
