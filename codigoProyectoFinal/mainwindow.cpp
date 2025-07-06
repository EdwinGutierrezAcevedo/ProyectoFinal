#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "goku.h"
#include "enemigo.h"
#include "gameobject.h"
#include <QTimer>
#include <QDebug>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Configurar escena gráfica
    scene = new QGraphicsScene(this);
    configurarEscena();

    // Iniciar bucle del juego
    iniciarBucleJuego();
}

void MainWindow::configurarEscena() {
    // Configurar vista gráfica
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Fondo panorámico (1184x150) en PNG
    QPixmap imagenFondo(":/img/Base.gif");
    ui->graphicsView->setBackgroundBrush(QBrush(imagenFondo));
    ui->graphicsView->scale(2.5,2.5);

    // Ajustar relación de aspecto
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Configurar escena para el fondo panorámico
    const int SCENE_WIDTH = 1184;
    const int SCENE_HEIGHT = 600; // Alto extendido

    scene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);

    // Crear personaje principal (Goku)
    goku = new Goku(); // Guardar referencia como miembro de la clase
    goku->setPos(90, 90); // Posición inicial sobre el suelo
    scene->addItem(goku);
    goku->setFocus(); // Para recibir eventos de teclado

    // Centrar inicialmente en Goku
    ui->graphicsView->centerOn(goku);

    Enemigo* soldado = new Enemigo(Enemigo::SOLDADO, goku);
    soldado->setPos(210, 90);
    scene->addItem(soldado);
}

void MainWindow::iniciarBucleJuego() {
    QTimer* gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [this]() {
        // Fase 0: Actualización lógica
        for (auto item : scene->items()) {
            if (auto obj = dynamic_cast<GameObject*>(item)) {
                obj->avanzar(0);
            }
        }

        // Fase 1: Detección de colisiones
        for (auto item : scene->items()) {
            if (auto obj = dynamic_cast<GameObject*>(item)) {
                // Verificar colisiones con otros objetos
                QList<QGraphicsItem*> itemsColisionados = obj->collidingItems();
                for (auto otro : itemsColisionados) {
                    if (auto otroObj = dynamic_cast<GameObject*>(otro)) {
                        obj->manejarColision(otroObj);
                    }
                }
            }
        }

        // Fase 2: Actualización gráfica
        for (auto item : scene->items()) {
            if (auto obj = dynamic_cast<GameObject*>(item)) {
                obj->avanzar(1);
            }
        }

        // Centrar cámara en Goku (después de todas las actualizaciones)
        ui->graphicsView->centerOn(goku);

        scene->update();
    });
    gameTimer->start(16);  // ~60 FPS
}

MainWindow::~MainWindow() {
    delete ui;
}
