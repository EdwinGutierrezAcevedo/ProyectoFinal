#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "goku.h"
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
    QPixmap imagenFondo(":/img/Base1.gif");
    ui->graphicsView->setBackgroundBrush(QBrush(imagenFondo));
    ui->graphicsView->scale(2.5,2.5);
    // Ajustar relación de aspecto
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Configurar escena para el fondo panorámico
    const int SCENE_WIDTH = 1184;
    const int SCENE_HEIGHT = 600; // Alto extendido

    scene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    // Crear personaje principal (Goku) sobre el suelo
    Goku* goku = new Goku();
    goku->setPos(90, 90); // Posición inicial sobre el suelo
    scene->addItem(goku);
    goku->setFocus(); // Para recibir eventos de teclado
}

void MainWindow::iniciarBucleJuego() {
    QTimer* gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [this]() {
        // Obtener UNA copia de los items por frame (eficiente)
        const auto& currentItems = scene->items();

        // Fase 0: Actualización lógica
        for (auto item : currentItems) {
            if (auto obj = dynamic_cast<GameObject*>(item)) {
                obj->avanzar(0);
            }
        }

        // Fase 1: Actualización gráfica
        for (auto item : currentItems) {
            if (auto obj = dynamic_cast<GameObject*>(item)) {
                obj->avanzar(1);
            }
        }

        scene->update();
    });
    gameTimer->start(16);
}

MainWindow::~MainWindow() {
    delete ui;
}
