#include "shapeitem.h"


ShapeItem::ShapeType ShapeItem::getNextShapeType(const ShapeType &type)
{
    if (type == LAST)
        return FIRST;

    return (ShapeType)((int)type + 1);
}

//=========================================================================================

ShapeItem::ShapeItem(qreal x, qreal y, qreal size, ShapeType type, QObject *parent)
    :
      QObject(parent),
      x(x - size / 2),
      y(y - size / 2),
      size(size),
      scale(1),
      type(type),
      isMoving(false),
      isRotating(false)
{
    QColor color(rand()%255, rand()%255, rand()%255);
    pen.setColor(color);
    brush.setColor(color);
    brush.setStyle(Qt::BrushStyle::SolidPattern);

    applyScale(scale);

    baseTransform = transform();
}

//=========================================================================================

void ShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen);
    painter->setBrush(brush);

    switch (type)
    {
    case RECTANGLE:
        painter->drawRect(scaledX, scaledY, scaledSize, scaledSize);
        break;
    case ELLIPSE:
        painter->drawEllipse(scaledX, scaledY, scaledSize, scaledSize);
        break;
    case STAR:
        painter->drawPolygon(starPolygon);
        break;
    }

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF ShapeItem::boundingRect() const
{
    return QRectF(scaledX, scaledY, scaledSize, scaledSize);
}

void ShapeItem::applyScale(float scale)
{
    qreal newSize = size * scale;
    qreal shift = (size - newSize) / 2;
    scaledX = x + shift;
    scaledY = y + shift;
    scaledSize = newSize;

    starPolygon = createStarPolygon();
}

QPolygon ShapeItem::createStarPolygon()
{
    QPolygon polygon;

    qreal leftX = scaledX;
    qreal topY = scaledY;
    qreal rightX = leftX + scaledSize;
    qreal bottomY = topY + scaledSize;

    polygon << QPoint(leftX, topY)
            << QPoint(leftX + scaledSize / 2, topY + scaledSize / 3)
            << QPoint(rightX, topY)
            << QPoint(rightX - scaledSize / 3, topY + scaledSize / 2)
            << QPoint(rightX, bottomY)
            << QPoint(leftX + scaledSize / 2, bottomY - scaledSize / 3)
            << QPoint(leftX, bottomY)
            << QPoint(leftX + scaledSize / 3, topY + scaledSize / 2);

    return polygon;
}

//=========================================================================================

void ShapeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button())
    {
        case Qt::RightButton:
            emit rightClicked(this);
            break;
        case Qt::LeftButton:
            isMoving = true;
            prevMousePos = event->pos().toPoint();
            break;
        case Qt::MiddleButton:
            isRotating = true;
            prevMousePos = event->pos().toPoint();
            break;
        default:
            break;
    }
}

void ShapeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button())
    {
        case Qt::LeftButton:
            isMoving = false;
            break;
        case Qt::MiddleButton:
            isRotating = false;
            break;
        default:
            break;
    }
}

void ShapeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isMoving)
    {
        QPoint shift = event->pos().toPoint() - prevMousePos;
        x += shift.rx();
        y += shift.ry();
        prevMousePos = event->pos().toPoint();
        emit redraw();
    }
    else if (isRotating)
    {
        QPoint shift = event->pos().toPoint() - prevMousePos;

        setTransformOriginPoint(scaledX, scaledY);
        QTransform newTransform = transform();


        if (shift.rx() > 0)
            newTransform.rotate(1);
        else
            newTransform.rotate(-1);

        setTransform(newTransform);
    }
}

void ShapeItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (event->delta() > 0)
        scale += 0.1;
    else
        scale -= 0.1;

    if (scale < 0.2)
        scale = 0.2;

    applyScale(scale);
    emit redraw();
}

