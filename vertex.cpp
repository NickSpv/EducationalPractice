#include "vertex.h"

Vertex::Vertex()
{

}

QRectF Vertex::boundingRect() const
{
    return QRectF (-30,-30,60,60);
}

void Vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::darkGreen);
    painter->drawEllipse(-15,-15,30,30);
    painter->setPen(Qt::white);
    painter->drawText(QRect(-25, -25, 50, 50), Qt::AlignCenter, QString::number(data));
    painter->setPen(Qt::darkBlue);
    painter->drawText(QRect(-25, -25, 20, 20), Qt::AlignCenter, QString::number(vertex_num));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Vertex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(mapToScene(event->pos()));
    for (auto line : begining_lines) {
        QLineF l = line->line();
        line->setLine(this->x(), this->y(), l.x2(), l.y2());
    }
    for (auto line : ending_lines) {
        QLineF l = line->line();
        line->setLine(l.x1(), l.y1(), this->x(), this->y());
    }
}

void Vertex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    Q_UNUSED(event);
}

void Vertex::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
    Q_UNUSED(event);
}

void Vertex::highlight()
{
    effect = new QGraphicsDropShadowEffect;
    this->setGraphicsEffect(effect);
}

void Vertex::unhighlight()
{
    this->setGraphicsEffect(0);
}
