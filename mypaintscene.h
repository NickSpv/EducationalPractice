#ifndef MYPAINTSCENE_H
#define MYPAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTextStream>
#include <QPainter>

class myPaintScene : public QGraphicsScene
{
    Q_OBJECT

public:
    myPaintScene();

private:
    bool isMousePressed;

    QPointF prev_point;
    int pen_size;
    QColor pen_color;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void cSliderChanged(int newValue);
    void cColorChanged(QColor newColor);

};

#endif // MYPAINTSCENE_H
