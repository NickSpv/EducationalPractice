#ifndef VERTEX_H
#define VERTEX_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QGraphicsDropShadowEffect>

class Vertex : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Vertex();
    void highlight();
    void unhighlight();

    QGraphicsEffect *effect;
    int data;
    int vertex_num;
    QList<Vertex*> adjacent_vertices;
    QList<QGraphicsLineItem*> begining_lines;
    QList<QGraphicsLineItem*> ending_lines;

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // VERTEX_H
