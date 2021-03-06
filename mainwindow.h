#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include "shapeitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QGraphicsScene* scene;

    ShapeItem::ShapeType nextShapeType;

    qreal scale;

    QTransform baseTransform;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *event) override;


private:
    Ui::MainWindow *ui;

public slots:
    void onRedraw();
    void onItemRightClicked(ShapeItem* item);
    bool eventFilter(QObject *object, QEvent *event) override;
};
#endif // MAINWINDOW_H
