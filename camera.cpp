#include"camera.h"
#include"myutil.h"
#include "QImage"
#include<QDebug>
#include <QLabel>
Camera::Camera(){
opened=false;
currentFrameNum=-1;
frameCount=-1;
videoRate=-1;
}
void Camera::open(){
    if(opened==false){
         cap=new cv::VideoCapture(0);
         if(cap!=NULL&&cap->isOpened()){
             opened=true;

         }
    }
}
void Camera::openVideo(std::string videoName){
    if(opened==false){
            cap=new cv::VideoCapture(videoName);
            if(cap!=NULL&&cap->isOpened()){
                opened=true;
                frameCount=cap->get(CV_CAP_PROP_FRAME_COUNT);
                videoRate=cap->get(CV_CAP_PROP_FPS);
            }
    }

}
long Camera::getCurrentFrameNum(){
    return currentFrameNum;
}
long Camera::getFrameCount(){
    return frameCount;
}
double Camera::getVideoRate(){
    return videoRate;
}

void Camera::nextFrame()
{
    if(opened==false){
        return;
    }
    currentFrameNum++;
    cv::Mat img;
    *cap>>img;
    emit transImg(img,currentFrameNum);
}
void Camera::close(){
    opened=false;
    delete cap;
    cap=NULL;
}
