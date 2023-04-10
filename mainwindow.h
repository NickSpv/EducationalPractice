#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mypaintscene.h>
#include <vertex.h>

#include <QList>
#include <QPair>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsLineItem>
#include <QFileDialog>
#include <QQueue>
#include <QThread>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

private:
    void generateImageGraphScene();

    Ui::MainWindow *ui;
    myPaintScene *drawingScene;
    QGraphicsScene *graphScene;

    int image_count;
    int graph_image_count;
    int minimum;

    QList<Vertex*> vertices;

public slots:
    void sMovedVertex(Vertex*);

signals:
    void sliderChanged(int newValue);
    void colorChanged(QColor newColor);
};

#endif // MAINWINDOW_H
