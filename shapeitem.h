#ifndef SHAPEITEM_H
#define SHAPEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QContextMenuEvent>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

class ShapeItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    enum ShapeType
    {
        RECTANGLE,
        ELLIPSE,
        STAR,
        FIRST = RECTANGLE,
        LAST = STAR
    };

    static ShapeType getNextShapeType(const ShapeType& type);

private:
    qreal x;
    qreal y;
    qreal size;

    qreal scaledX;
    qreal scaledY;
    qreal scaledSize;

    float scale;

    ShapeType type;

    QPen pen;
    QBrush brush;

    QPolygon starPolygon;

    bool isMoving;
    QPoint prevMousePos;

    bool isRotating;

    QTransform baseTransform;

public:
    explicit ShapeItem(qreal x, qreal y, qreal size, ShapeType type, QObject *parent = nullptr);


private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void applyScale(float scale);
    QPolygon createStarPolygon();

signals:
    void redraw();
    void rightClicked(ShapeItem* item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

};

#endif // SHAPEITEM_H
