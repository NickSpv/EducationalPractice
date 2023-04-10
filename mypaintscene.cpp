#include "mypaintscene.h"

myPaintScene::myPaintScene()
{
    isMousePressed = false;
    this->setSceneRect(0, 0, 10, 10);
    pen_size = 1;
    pen_color = Qt::black;
}

void myPaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    addEllipse(event->scenePos().x() - 5, event->scenePos().y() - 5,
               pen_size, pen_size, QPen(Qt::NoPen), QBrush(pen_color));
    prev_point = event->scenePos();
    isMousePressed = true;
}

void myPaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isMousePressed) {
        addLine(prev_point.x(), prev_point.y(),
                event->scenePos().x(), event->scenePos().y(),
                QPen(pen_color,pen_size,Qt::SolidLine,Qt::RoundCap));
        prev_point = event->scenePos();
    }
}

void myPaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    addEllipse(event->scenePos().x() - 5, event->scenePos().y() - 5,
               pen_size, pen_size, QPen(Qt::NoPen), QBrush(pen_color));
    prev_point = event->scenePos();
    isMousePressed = false;
}

void myPaintScene::cSliderChanged(int value) {
    pen_size = value;
}

void myPaintScene::cColorChanged(QColor newColor) {
    pen_color = newColor;
}
