#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "goku.h"
#include "enemigo.h"
#include "gameobject.h"
#include "nivel.h" // Incluir nueva clase
#include <QTimer>
#include <QDebug>
#include <QResizeEvent>
#include <QPushButton>
#include <QGraphicsProxyWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Configurar escena gráfica
    scene = new QGraphicsScene(this);
    configurarEscena();

    // Iniciar con nivel 1
    nivelActual = new Nivel(Nivel::NIVEL1, goku, scene, this);
    connect(nivelActual, &Nivel::completado, this, &MainWindow::mostrarCompletadoNivel);
    nivelActual->iniciar();

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
    goku = new Goku();
    qDebug() << "se creo goku";
    goku->setPos(90, 90);
    scene->addItem(goku);
    goku->setFocus();

    // Centrar inicialmente en Goku
    ui->graphicsView->centerOn(goku);
}

void MainWindow::mostrarCompletadoNivel() {
    // Obtener el centro de la vista (que es donde está Goku)
    QRectF viewRect = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->geometry()).boundingRect();
    QPointF center = viewRect.center();

    // Mostrar texto de nivel completado
    QGraphicsTextItem* texto = new QGraphicsTextItem();
    texto->setPlainText(QString("Nivel %1 Superado!").arg(nivelActual->numero() + 1));
    texto->setDefaultTextColor(Qt::yellow);
    texto->setFont(QFont("Arial", 24, QFont::Bold));

    // Centrar el texto en la vista
    QRectF textRect = texto->boundingRect();
    texto->setPos(center.x() - textRect.width()/2, center.y() - 50);
    scene->addItem(texto);

    // Crear botón para siguiente nivel
    QPushButton* botonSiguiente = new QPushButton();
    botonSiguiente->setText("Ir al Nivel " + QString::number(nivelActual->numero() + 2));
    botonSiguiente->setStyleSheet("background-color: #FF5500; color: white; font-weight: bold;");
    botonSiguiente->setFixedSize(200, 50);

    QGraphicsProxyWidget* proxy = scene->addWidget(botonSiguiente);
    // Centrar el botón debajo del texto
    proxy->setPos(center.x() - 100, center.y() + 20);

    // Guardar elementos en variables miembro para limpieza posterior
    elementosTransitorios.append(texto);
    elementosTransitorios.append(proxy);

    connect(botonSiguiente, &QPushButton::clicked, this, [this](){
        // Limpiar elementos de transición
        auto it = elementosTransitorios.begin();
        while (it != elementosTransitorios.end()) {
            QObject* item = *it;
            if (QGraphicsItem* gi = dynamic_cast<QGraphicsItem*>(item)) {
                scene->removeItem(gi);
                delete gi;
                it = elementosTransitorios.erase(it); // Eliminar de la lista y avanzar
            }
            else if (QGraphicsProxyWidget* proxy = dynamic_cast<QGraphicsProxyWidget*>(item)) {
                scene->removeItem(proxy);
                proxy->deleteLater();
                it = elementosTransitorios.erase(it); // Eliminar de la lista y avanzar
            } else {
                ++it; // Avanzar sin eliminar
            }
        }
        elementosTransitorios.clear();

        // Avanzar al siguiente nivel
        avanzarSiguienteNivel();
    });
}

void MainWindow::avanzarSiguienteNivel() {
    // Guardar número antes de eliminar
    int numSiguiente = nivelActual->numero() + 1;

    // Limpiar nivel actual
    nivelActual->limpiar();
    delete nivelActual;
    nivelActual = nullptr;

    // Crear nuevo nivel
    nivelActual = new Nivel(static_cast<Nivel::NumeroNivel>(numSiguiente), goku, scene, this);
    connect(nivelActual, &Nivel::completado, this, &MainWindow::mostrarCompletadoNivel);
    nivelActual->iniciar();

    goku->setFocus();
}

void MainWindow::iniciarBucleJuego() {
    QTimer* gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [this]() {
        // Solo actualizar si no estamos en transición entre niveles
        if (nivelActual && !nivelActual->estaCompletado()) {
            // Fase 0: Actualización lógica
            const QList<QGraphicsItem*> items = scene->items();
            for (auto it = items.begin(); it != items.end(); ++it) {
                if (auto obj = dynamic_cast<GameObject*>(*it)) {
                    obj->avanzar(0);
                }
            }
            /*
            // Fase 1: Detección de colisiones
            for (auto it = items.begin(); it != items.end(); ++it) {
                if (auto obj = dynamic_cast<GameObject*>(*it)) {
                    // Verificar colisiones con otros objetos
                    const QList<QGraphicsItem*> itemsColisionados = obj->collidingItems();
                    for (auto colIt = itemsColisionados.begin(); colIt != itemsColisionados.end(); ++colIt) {
                        if (auto otroObj = dynamic_cast<GameObject*>(*colIt)) {
                            obj->manejarColision(otroObj);
                        }
                    }
                }
            }
            */
            // Fase 2: Actualización gráfica
            for (auto it = items.begin(); it != items.end(); ++it) {
                if (auto obj = dynamic_cast<GameObject*>(*it)) {
                    obj->avanzar(1);
                }
            }
        }

        // Centrar cámara en Goku
        ui->graphicsView->centerOn(goku);

        scene->update();
    });
    gameTimer->start(16);  // ~60 FPS
}

MainWindow::~MainWindow() {
    delete ui;
}
