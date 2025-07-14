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
    connect(goku, &Goku::gameOver, this, &MainWindow::mostrarGameOver);
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
    QRectF viewRect = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->geometry()).boundingRect();
    QPointF center = viewRect.center();

    QGraphicsTextItem* texto = new QGraphicsTextItem();

    // Verificar si es el último nivel (nivel 2)
    if (nivelActual->numero() == Nivel::NIVEL2) {
        texto->setPlainText("Juego terminado");
    } else {
        texto->setPlainText(QString("Nivel %1 Superado!").arg(nivelActual->numero() + 1));
    }

    texto->setDefaultTextColor(Qt::yellow);
    texto->setFont(QFont("Arial", 24, QFont::Bold));
    QRectF textRect = texto->boundingRect();
    texto->setPos(center.x() - textRect.width()/2, center.y() - 50);
    scene->addItem(texto);

    QPushButton* botonAccion = new QPushButton();

    if (nivelActual->numero() == Nivel::NIVEL2) {
        botonAccion->setText("Reiniciar Juego");
    } else {
        botonAccion->setText("Ir al Nivel " + QString::number(nivelActual->numero() + 2));
    }

    botonAccion->setStyleSheet("background-color: #FF5500; color: white; font-weight: bold;");
    botonAccion->setFixedSize(200, 50);

    QGraphicsProxyWidget* proxy = scene->addWidget(botonAccion);
    proxy->setPos(center.x() - 100, center.y() + 20);

    elementosTransitorios.append(texto);
    elementosTransitorios.append(proxy);

    // Conectar según el tipo de acción
    if (nivelActual->numero() == Nivel::NIVEL2) {
        connect(botonAccion, &QPushButton::clicked, this, [this](){
            limpiarElementosTransitorios();
            reiniciarJuego();
        });
    } else {
        connect(botonAccion, &QPushButton::clicked, this, [this](){
            limpiarElementosTransitorios();
            avanzarSiguienteNivel();
        });
    }
}

void MainWindow::mostrarGameOver() {
    // Limpiar nivel actual
    if (nivelActual) {
        nivelActual->limpiar();
        delete nivelActual;
        nivelActual = nullptr;
    }

    // Detener bucle de juego
    if (gameTimer && gameTimer->isActive()) {
        gameTimer->stop();
    }

    // Obtener centro de la vista
    QRectF viewRect = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->geometry()).boundingRect();
    QPointF center = viewRect.center();

    // Mostrar texto de juego terminado
    QGraphicsTextItem* texto = new QGraphicsTextItem();
    texto->setPlainText("Juego terminado");
    texto->setDefaultTextColor(Qt::red);
    texto->setFont(QFont("Arial", 24, QFont::Bold));

    QRectF textRect = texto->boundingRect();
    texto->setPos(center.x() - textRect.width()/2, center.y() - 50);
    scene->addItem(texto);

    // Crear botón para reiniciar
    QPushButton* botonReiniciar = new QPushButton();
    botonReiniciar->setText("Reiniciar Juego");
    botonReiniciar->setStyleSheet("background-color: #FF5500; color: white; font-weight: bold;");
    botonReiniciar->setFixedSize(200, 50);

    QGraphicsProxyWidget* proxy = scene->addWidget(botonReiniciar);
    proxy->setPos(center.x() - 100, center.y() + 20);

    // Guardar elementos en lista para limpieza
    elementosTransitorios.append(texto);
    elementosTransitorios.append(proxy);

    connect(botonReiniciar, &QPushButton::clicked, this, [this](){
        limpiarElementosTransitorios();
        reiniciarJuego();
    });
}

void MainWindow::avanzarSiguienteNivel() {
    // Guardar número antes de eliminar
    int numSiguiente = nivelActual->numero() + 1;
    if (numSiguiente > Nivel::NIVEL2) return;

    // Limpiar nivel actual
    nivelActual->limpiar();
    delete nivelActual;  // Esto llama al destructor que marca m_destruyendo
    nivelActual = nullptr;

    // Crear nuevo nivel
    nivelActual = new Nivel(static_cast<Nivel::NumeroNivel>(numSiguiente), goku, scene, this);
    connect(nivelActual, &Nivel::completado, this, &MainWindow::mostrarCompletadoNivel);
    nivelActual->iniciar();

    goku->setFocus();
}

void MainWindow::reiniciarJuego() {
    // Limpiar elementos transitorios (si existen)
    limpiarElementosTransitorios();

    // Limpiar nivel actual si existe
    if (nivelActual) {
        nivelActual->limpiar();
        delete nivelActual;
        nivelActual = nullptr;
    }

    // Eliminar Goku viejo (si existe)
    if (goku) {
        scene->removeItem(goku);
        delete goku;
        goku = nullptr;
    }

    // Crear nuevo Goku
    goku = new Goku();
    goku->setPos(90, 90);
    scene->addItem(goku);
    goku->setFocus();
    connect(goku, &Goku::gameOver, this, &MainWindow::mostrarGameOver);

    // Crear nuevo nivel 1
    nivelActual = new Nivel(Nivel::NIVEL1, goku, scene, this);
    connect(nivelActual, &Nivel::completado, this, &MainWindow::mostrarCompletadoNivel);
    nivelActual->iniciar();

    // Centrar vista en Goku
    ui->graphicsView->centerOn(goku);

    // Reiniciar bucle de juego si estaba detenido
    if (gameTimer && !gameTimer->isActive()) {
        gameTimer->start();
    }
}

void MainWindow::limpiarElementosTransitorios() {
    for (auto* item : elementosTransitorios) {
        if (auto* gi = dynamic_cast<QGraphicsItem*>(item)) {
            scene->removeItem(gi);
            delete gi;
        }
        else if (auto* proxy = dynamic_cast<QGraphicsProxyWidget*>(item)) {
            // Eliminar widget primero
            if (proxy->widget()) {
                delete proxy->widget();
            }
            scene->removeItem(proxy);
            delete proxy;
        }
    }
    elementosTransitorios.clear();
}


void MainWindow::iniciarBucleJuego() {
    gameTimer = new QTimer(this);
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
