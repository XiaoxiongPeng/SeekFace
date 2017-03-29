#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <QTimer>
#include <camera.h>
#include<seetaface.h>
#include <QThreadPool>
#include "logger.h"
class Controller:public QObject{
    Q_OBJECT
public :  
    Controller();

private:
   cv::Mat srcFaceImg;
   Camera* camera;
   QTimer* timer;
   SeetaFace* seetaFace;
   float* srcFeat;
   QThreadPool* threadPool;
   Logger* logger;
public slots:
    void openCamera();
    void closeCamera();
    void openVideo( QString videoName);
    void openSeekFace();
    void chooseImg();
    void imgProcess(cv::Mat &img,long frameNum);
    void seekFace(cv::Mat& img);
    void test();
    void setSrcImg(cv::Mat img);
    void foundFace(float result,long frameNum);
signals:
    void transImg(cv::Mat& img);
    void sendFaceInfo(QString info);
};

#endif // CONTROLLER_H
