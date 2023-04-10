#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    drawingScene = new myPaintScene();
    graphScene = new QGraphicsScene(this);

    ui->graphicsView->setScene(drawingScene);
    ui->graphicsView_graph->setScene(graphScene);

    ui->graphicsView_graph->setSceneRect(0, 0, 500, 500);

    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setMaximum(50);
    image_count = 0;
    graph_image_count = 0;

    connect(this, SIGNAL(sliderChanged(int)),
            drawingScene, SLOT(cSliderChanged(int)));
    connect(this, SIGNAL(colorChanged(QColor)),
            drawingScene, SLOT(cColorChanged(QColor)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete drawingScene;
    delete graphScene;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    emit sliderChanged(value);
}

void MainWindow::on_pushButton_2_clicked()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()) {
        emit colorChanged(color);
    }
}

void MainWindow::on_pushButton_clicked()
{
    drawingScene->clearSelection();
    drawingScene->setSceneRect(drawingScene->itemsBoundingRect());
    QImage image(drawingScene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    drawingScene->render(&painter);
    QString image_name = "./images/image%1.png";
    image.save(image_name.arg(image_count));
    image_count++;
}

void MainWindow::on_pushButton_3_clicked()
{
    drawingScene->clear();
}

void MainWindow::on_pushButton_8_clicked()
{
    Vertex *vertex = new Vertex();
    vertex->data = QInputDialog::getInt(this, tr("Добавление вершины"), tr("Вес вершины:"), 1, 1, 100);
    vertex->vertex_num = vertices.size();
    vertex->setPos(QRandomGenerator::global()->bounded(30, 300), QRandomGenerator::global()->bounded(30, 300));
    vertices.append(vertex);
    graphScene->addItem(vertex);
}


void MainWindow::on_pushButton_9_clicked()
{
    if (vertices.size() < 2) {
        QMessageBox allert;
        allert.setText("Недостаточно вершин!!!");
        allert.exec();
        return;
    }
    int left_vertex = QInputDialog::getInt(this, tr("Добавление ребра"),
                                           tr("Введите номер вершины из которой ребро выходит:"),
                                           0, 0, vertices.size() - 1);
    int right_vertex = QInputDialog::getInt(this, tr("Добавление ребра"),
                                            tr("Введите номер вершины в которую ребро входит:"),
                                            0, 0, vertices.size() - 1);
    if (left_vertex == right_vertex) {
        QMessageBox allert;
        allert.setText("Введите разные вершины!!!");
        allert.exec();
        return;
    }

    Vertex *right, *left;

    for (auto current_vertex : vertices) {
        if (current_vertex->vertex_num == left_vertex) {
            left = current_vertex;
        }
        if (current_vertex->vertex_num == right_vertex) {
            right = current_vertex;
        }
    }

    right->adjacent_vertices.append(left);
    left->adjacent_vertices.append(right);

    QGraphicsLineItem *line = new QGraphicsLineItem(left->x(), left->y(), right->x(), right->y());
    graphScene->addItem(line);
    right->ending_lines.append(line);
    left->begining_lines.append(line);
}

void MainWindow::sMovedVertex(Vertex* vertex)
{
    for (auto curr_vertex : vertex->adjacent_vertices) {
        graphScene->addLine(vertex->x(), vertex->y(), curr_vertex->x(), curr_vertex->y());
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    QTextStream out(stdout);
    QString file_path = QFileDialog::getOpenFileName(0, "Выберите файл", "");
    QFile file(file_path);
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        QMessageBox allert;
        allert.setText("Ошибка открытия файла!!!");
        allert.exec();
        return;
    }
    QTextStream fileStream(&file);
    QString fline = fileStream.readLine();
    while (fline != "***") {
        Vertex *vertex = new Vertex();
        vertex->data = fline.toInt();
        vertex->vertex_num = vertices.size();
        vertex->setPos(QRandomGenerator::global()->bounded(30, 300), QRandomGenerator::global()->bounded(30, 300));
        vertices.append(vertex);
        graphScene->addItem(vertex);
        fline = fileStream.readLine();
    }
    while (!fileStream.atEnd()) {
        fline = fileStream.readLine();
        int left_vertex = fline.section(' ', 0, 0).toInt();
        int right_vertex = fline.section(' ', 1, 1).toInt();

        Vertex *right, *left;

        for (auto current_vertex : vertices) {
            if (current_vertex->vertex_num == left_vertex) {
                left = current_vertex;
            }
            if (current_vertex->vertex_num == right_vertex) {
                right = current_vertex;
            }
        }

        right->adjacent_vertices.append(left);
        left->adjacent_vertices.append(right);

        QGraphicsLineItem *line = new QGraphicsLineItem(left->x(), left->y(), right->x(), right->y());
        graphScene->addItem(line);
        right->ending_lines.append(line);
        left->begining_lines.append(line);
    }
}


void MainWindow::on_pushButton_6_clicked()
{
    graph_image_count = 0;
    if (vertices.empty()) {
        QMessageBox allert;
        allert.setText("Граф не задан!!!");
        allert.exec();
        return;
    }
    Vertex* min = vertices[0];
    bool flag;
    QQueue<Vertex*> queue;
    QMap<Vertex*, bool> visited;
    queue.append(vertices[0]);
    visited[vertices[0]] = true;
    do {
        flag = false;
        while (!queue.empty()) {
            Vertex* current_vertex = queue.front();
            queue.pop_front();
            current_vertex->highlight();
            generateImageGraphScene();
            current_vertex->unhighlight();
            if (current_vertex->data < min->data) {
                min = current_vertex;
            }
            for (auto vertex : current_vertex->adjacent_vertices) {
                if (!visited[vertex]) {
                    visited[vertex] = true;
                    queue.append(vertex);
                }
            }
        }

        for (auto vert : vertices) {
            if (!visited[vert]) {
                flag = true;
                queue.append(vert);
                visited[vert] = true;
            }
        }

    } while (flag);
    this->minimum = min->data;
    QMessageBox allert;
    allert.setText("Минимальный вес элемента (вершины) графа графа: " + QString::number(min->data));
    allert.exec();
}

void MainWindow::generateImageGraphScene()
{
    graphScene->clearSelection();
    graphScene->setSceneRect(drawingScene->itemsBoundingRect());
    QImage image(graphScene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    graphScene->render(&painter);
    QString image_name = "./images_for_graph/image%1.png";
    image.save(image_name.arg(graph_image_count));
    graph_image_count++;
}

void MainWindow::on_pushButton_5_clicked()
{
    QFile out("./html/index.html");
    out.open(QIODevice::WriteOnly);
    QTextStream output(&out);
    output << "<!DOCTYPE html>\n";
    output << "<html>\n";
    output << "<head>\n";
    output << "<meta charset=\"utf-8\">\n";
    output << "<title>Stebenev Pavel. V15</title>\n";
    output << "</head>\n<body>";
    output << "<h1 style=\"color:#000000\">V15. Stebenev Pavel. PI-21-1</h1>";
    output << "<p style=\"color:#000000\">The search for the minimum element of the graph is performed by BFS.</p>";
    for (int i = 0; i < graph_image_count; i++) {
        output << "<br>\n";
        output << "<h4 style=\"color:#000000\">Iteration " << i << "</h4>\n";
        QString img = "<img src=\"/home/Pavel/Документы/Универ/Учебная практика/build-Educational"
                      "Practice-Desktop-Debug/images_for_graph/image%1.png\" alt=\"Изображение\">\n";
        output << img.arg(i);
    }
    output << "<h3 style=\"color:#000000\">Minimum is " << minimum << " </h3>";
    output << "</body>\n</html>";
    out.close();
    QFile file("./html/index.html");
    file.open(QIODevice::ReadOnly);
    QString html = file.readAll();
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setHtml(html);
    ui->tabWidget->setCurrentIndex(2);
}

