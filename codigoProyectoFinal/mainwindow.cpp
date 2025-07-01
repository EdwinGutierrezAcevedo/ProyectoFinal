#include "mainwindow.h"
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
