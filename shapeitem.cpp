#include "shapeitem.h"


ShapeItem::ShapeType ShapeItem::getNextShapeType(const ShapeType &type)
{
    if (type == LAST)
        return FIRST;

    return (ShapeType)((int)type + 1);
}

ShapeItem::ShapeItem(qreal x, qreal y, qreal size, ShapeType type, QObject *parent)
    :
      QObject(parent),
      x(x - size / 2),
      y(y - size / 2),
      size(size),
      type(type)
{
    QColor color(rand()%255, rand()%255, rand()%255);
    pen.setColor(color);
    brush.setColor(color);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
}

//=========================================================================================

void ShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug(__FUNCTION__);

    painter->setPen(pen);
    painter->setBrush(brush);

    switch (type)
    {
    case RECTANGLE:
        painter->drawRect(x, y, size, size);
        break;
    case ELLIPSE:
        painter->drawEllipse(x, y, size, size);
        break;
    case STAR:

        QPolygon polygon;

        qreal leftX = x;
        qreal topY = y;
        qreal rightX = x + size;
        qreal bottomY = y + size;

        polygon << QPoint(leftX, topY)
                << QPoint(leftX + size / 2, topY + size / 3)
                << QPoint(rightX, topY)
                << QPoint(rightX - size / 3, topY + size / 2)
                << QPoint(rightX, bottomY)
                << QPoint(leftX + size / 2, bottomY - size / 3)
                << QPoint(leftX, bottomY)
                << QPoint(leftX + size / 3, topY + size / 2);

        painter->drawPolygon(polygon);

        break;
    }

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF ShapeItem::boundingRect() const
{
    return QRectF(x, y, size, size);
}

//=========================================================================================

void ShapeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        emit rightClicked(this);
    else if (event->button() == Qt::LeftButton)
    {
        isMoving = true;
        prevMousePos = event->pos().toPoint();
    }
}

void ShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        isMoving = false;
}

void ShapeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isMoving)
    {
        qDebug(__FUNCTION__);
        QPoint shift = event->pos().toPoint() - prevMousePos;

        qDebug() << shift;

        x += shift.rx();
        y += shift.ry();

        prevMousePos = event->pos().toPoint();
        emit redraw();
    }
}

void ShapeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug(__FUNCTION__);
}
