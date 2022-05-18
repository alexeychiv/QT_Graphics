#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :
        QMainWindow(parent),
        nextShapeType(ShapeItem::RECTANGLE),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->viewport()->installEventFilter(this);

    scene = new QGraphicsScene();

    scene->setSceneRect(0, 0, 800, 600);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    srand(time(0));

    baseTransform = ui->graphicsView->transform();
    scale = 1;
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

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->graphicsView->viewport() && event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = dynamic_cast<QWheelEvent*>(event);

        if (!wheelEvent)
            return false;

        if (wheelEvent->angleDelta().ry() > 0)
            scale += 0.1;
        else
            scale -= 0.1;

        if (scale < 0.2)
            scale = 0.2;

        QTransform transform = baseTransform;
        transform.scale(scale, scale);
        ui->graphicsView->setTransform(transform);

        return true;
    }

    return false;
}
