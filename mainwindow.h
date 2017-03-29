#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMouseEvent>
#include <controller.h>
#include<QPoint>
#include <QStandardItemModel>
#include <QPaintEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:   
    QStandardItemModel* standardItemModel;
    explicit MainWindow(QWidget *parent = 0);
    void initialize();
    void paintEvent(QPaintEvent* event);
    ~MainWindow();
public slots:
    void freshImg(cv::Mat& img);
    void setDefaultImgForVideoLabel();
    void setDefaultImgForImgLabel();
    void showImg(cv::Mat img);
    void chooseImg();
    void openVideo();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void showResult(QString info);
signals:
    void transImg(cv::Mat img);
    void transVideoName(QString videoName);

private slots:

    void on_pushButton_shutdown_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_mini_clicked();

    void on_pushButton_Pause_clicked();

private:
    Ui::MainWindow *ui;
    Controller* controller;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;

};

#endif // MAINWINDOW_H
