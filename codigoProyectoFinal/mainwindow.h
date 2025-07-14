#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "goku.h"
#include "nivel.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Goku* goku;
    void ajustarVista();
    void configurarEscena();
    void iniciarBucleJuego();
    void centrarCamaraEnGoku();
    Nivel* nivelActual;
    QList<QObject*> elementosTransitorios;
    void limpiarElementosTransitorios();
    int score = 0;  // Nuevo: puntuación
    QTimer* gameTimer;
    QGraphicsTextItem* scoreText = nullptr;    // Para guardar elementos de transición
private slots:
    void mostrarCompletadoNivel();
    void mostrarGameOver();
    void avanzarSiguienteNivel();
    void reiniciarJuego();
};

#endif // MAINWINDOW_H
