#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myutil.h"
#include<QFileDialog>
#include <QDebug>
#include <QStandardItem>
#include <QPainter>
#include <QBrush>
#include <QRect>
#include <QColor>
#include <QFont>
#include <QPen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);  
     this->setWindowFlags(Qt::FramelessWindowHint);
     this->setAttribute(Qt::WA_TranslucentBackground);
     initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize(){
    controller=new Controller();
    standardItemModel=new QStandardItemModel();

   // this->setStyleSheet("QWidget{border-radius:15px;}");

    this->ui->listView_result->setModel(standardItemModel);
    this->ui->label_cv->setScaledContents(true);
    this->ui->label_chosenImg->setScaledContents(true);

    this->setDefaultImgForImgLabel();
    this->setDefaultImgForVideoLabel();

   //打开摄像头
    QObject::connect(controller,SIGNAL(transImg(cv::Mat&)),this,SLOT(freshImg(cv::Mat&)));
    QObject::connect(ui->pushButton_openCamera,SIGNAL(clicked()),controller,SLOT(openCamera()));
    //关闭摄像头
    QObject::connect(ui->pushButton_closeCamera,SIGNAL(clicked()),controller,SLOT(closeCamera()));
    QObject::connect(ui->pushButton_closeCamera,SIGNAL(clicked()),this,SLOT(setDefaultImgForVideoLabel()));
    //选择图片
    QObject::connect(ui->pushButton_chooseImage,SIGNAL(clicked()),this,SLOT(chooseImg()));
    QObject::connect(this,SIGNAL(transImg(cv::Mat)),controller,SLOT(setSrcImg(cv::Mat)));
    //打开视频
    QObject::connect(ui->pushButton_openVideo,SIGNAL(clicked()),this,SLOT(openVideo()));
    QObject::connect(this,SIGNAL(transVideoName(QString)),controller,SLOT(openVideo(QString)));

    QObject::connect(controller,SIGNAL(sendFaceInfo(QString)),this,SLOT(showResult(QString)));

}

void MainWindow::freshImg(cv::Mat& img){
    QImage qimg= MyUtil::Mat2QImage(img);
    this->ui->label_cv->setPixmap(QPixmap::fromImage(qimg));
}
void MainWindow::setDefaultImgForVideoLabel(){
    cv::Mat img=cv::imread("image/SHU.jpeg",1);
    QImage qimg= MyUtil::Mat2QImage(img);
    this->ui->label_cv->setPixmap(QPixmap::fromImage(qimg));
}
void MainWindow::setDefaultImgForImgLabel(){
    cv::Mat img=cv::imread("image/SHU.jpeg",1);
    QImage qimg= MyUtil::Mat2QImage(img);
    this->ui->label_chosenImg->setPixmap(QPixmap::fromImage(qimg));
}

void MainWindow::showImg(cv::Mat img){
    QImage qimg= MyUtil::Mat2QImage(img);
    this->ui->label_chosenImg->setPixmap(QPixmap::fromImage(qimg));
}
void MainWindow::chooseImg(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                       tr("choose image"),//对话框名称
                                        ".",//默认打开文件位置“.”文件目录"/"根目录
                                        tr("image files(*.jpg *.png *.bmp *.jpeg)"));//筛选器
    if(fileName.trimmed()!=""){
        cv::Mat img=cv::imread(fileName.toStdString(),1);       
        this->showImg(img);
        emit transImg(img);
    }

}
void MainWindow::openVideo(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                       tr("choose image"),//对话框名称
                                        ".",//默认打开文件位置“.”文件目录"/"根目录
                                        tr("video files(*.avi *.mp4)"));//筛选器
    if(fileName.trimmed()!=""){
        qDebug()<<"fileName: ";
        qDebug()<<fileName;
        emit transVideoName(fileName);
    }
}
void MainWindow::mousePressEvent(QMouseEvent *event){
         this->windowPos = this->pos();                // 获得部件当前位置
         this->mousePos = event->globalPos();     // 获得鼠标位置
         this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
     this->move(event->globalPos() - this->dPos);
}

void MainWindow::showResult(QString info){
    QStandardItem* item =new QStandardItem(info);
    standardItemModel->appendRow(item);

}
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QColor color;
    color.setRgb(255,255,255);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(color));
    //painter.pen().setWidth(5);
    painter.setPen(QColor(125,128,128));//(Qt::transparent);
    painter.setPen(Qt::SolidLine);
    painter.setPen(3);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
   // painter.drawPixmap(rect, QPixmap("F:/bgimg.jpg"));
    painter.drawRoundedRect(rect, 10, 10);

    painter.setPen(QColor(155,155,155));
    painter.drawLine(QPointF(0,33),QPointF(rect.width(),33));
    painter.drawLine(QPointF(0,77),QPointF(rect.width(),77));
    QRect rect1;
    rect1.setX(0);
    rect1.setY(5);
    rect1.setWidth(rect.width());
    rect1.setHeight(28);
   //painter.drawPixmap(rect1, QPixmap("F:/button.jpg"));

       // QPainterPath painterPath;

         //rect.setHeight(33);
         //painter.setBrush(QBrush(QColor(44,0,0)));
         //painter.setPen(QColor(255,255,255));
       // painterPath.addRoundedRect(rect, 8, 8);
        //painter.drawPath(painterPath);

      /* 绘制文字
       *  QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(50);
        font.setItalic(true);
        painter.setFont(font);
        painter.setPen(QColor(255,255,255));
        painter.drawText(rect, Qt::AlignLeft, "Qt");
    */
    QWidget::paintEvent(event);
}

void MainWindow::on_pushButton_shutdown_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_max_clicked()
{

    this->showMaximized();
}

void MainWindow::on_pushButton_mini_clicked()
{
   this->showMinimized();
}

void MainWindow::on_pushButton_Pause_clicked()
{

}
