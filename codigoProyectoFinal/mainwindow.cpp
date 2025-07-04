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
    // Configurar bucle del juego
    QTimer* gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, [this]() {
        // Fase 0: Actualización lógica
        for (auto item : scene->items()) {
            if (auto obj = dynamic_cast<GameObject*>(item)) {
                obj->avanzar(0);
            }
        }

        // Fase 1: Actualización gráfica
        for (auto item : scene->items()) {
            if (auto obj = dynamic_cast<GameObject*>(item)) {
                obj->avanzar(1);
            }
        }

        // Actualizar la vista
        scene->update();
    });
    gameTimer->start(16); // ≈60 FPS (1000ms/60 = 16.67ms)
}

MainWindow::~MainWindow() {
    delete ui;
}

/*#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QImage imagenFondo(":/img/Base.gif");
    if (imagenFondo.isNull()) {
        qDebug() << "La imagen no se cargó correctamente";
    }
    QBrush brochaFondo(imagenFondo);
    ui->graphicsView->setBackgroundBrush(brochaFondo);
    ui->graphicsView->scale(2,2);
    scene->setSceneRect(600,245,420,170);

}

MainWindow::~MainWindow()
{
    delete ui;
}
*/
