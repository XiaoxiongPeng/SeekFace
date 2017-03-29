#ifndef CAMERA_H
#define CAMERA_H
#include<qobject.h>
#include <opencv2/core/core.hpp>
#include <highgui/highgui.hpp>
class Camera:public QObject{
    Q_OBJECT
public:
    cv::VideoCapture* cap;
    Camera();
    volatile bool opened;
public slots:
    void open();
    void openVideo(std::string videoName);
    void close();
    void nextFrame();
    long getCurrentFrameNum();
    long getFrameCount();
    double getVideoRate();
signals:
    void transImg(cv::Mat &img,long currentFrameNum);
private:
    long frameCount;
    long currentFrameNum;
    double videoRate;

};

#endif // CAMERA_H
