#ifndef NIVEL_H
#define NIVEL_H

#include <QObject>
#include <QGraphicsScene>
#include "goku.h"
#include "enemigo.h"

class Nivel : public QObject {
    Q_OBJECT
public:
    enum NumeroNivel { NIVEL1, NIVEL2 };

    explicit Nivel(NumeroNivel numero, Goku* goku, QGraphicsScene* scene, QObject* parent = nullptr);
    ~Nivel(); // Destructor para limpieza

    void iniciar();
    void limpiar();

    bool estaCompletado() const;
    NumeroNivel numero() const;

signals:
    void completado();
    void enemigoEliminado();

private:
    void crearEnemigos();
    void verificarCompletado();

    NumeroNivel m_numero;
    Goku* m_goku;
    QGraphicsScene* m_scene;
    bool m_completado;
    QList<Enemigo*> m_enemigos;
    QTimer* m_verificador;

};

#endif // NIVEL_H
