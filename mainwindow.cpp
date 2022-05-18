#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :
        QMainWindow(parent),
        nextShapeType(ShapeItem::RECTANGLE),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();

    scene->setSceneRect(0, 0, 800, 600);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srand(time(0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPointF pos = ui->graphicsView->mapToScene(event->pos());

        ShapeItem* newItem = new ShapeItem(pos.rx(), pos.ry(), 100, nextShapeType);
        connect(newItem, &ShapeItem::rightClicked, this, &MainWindow::onItemRightClicked);
        connect(newItem, &ShapeItem::redraw, this, &MainWindow::onRedraw);
        scene->addItem(newItem);
        nextShapeType = ShapeItem::getNextShapeType(nextShapeType);
    }
}

void MainWindow::onRedraw()
{
    scene->update();
}

void MainWindow::onItemRightClicked(ShapeItem *item)
{
    scene->removeItem(item);
}
