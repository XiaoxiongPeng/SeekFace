#include "controller.h"
#include<QDebug>
#include<thread>
#include "worker.h"
Controller::Controller(){
    timer=new QTimer();
    camera=new Camera();
    logger=new Logger("log.txt");
    logger->log("-------------------------------------","","",000000000000000000);
    seetaFace=new SeetaFace();
    threadPool=new QThreadPool(this);
    threadPool->setMaxThreadCount(10);
    srcFeat=seetaFace->NewFeatureBuffer();
    QObject::connect(timer,SIGNAL(timeout()),camera,SLOT(nextFrame()));
    QObject::connect(camera,SIGNAL(transImg(cv::Mat&,long)),this,SLOT(imgProcess(cv::Mat&,long)));

}
void Controller::openCamera(){
   camera->open();
   timer->start(50);

}
void Controller::closeCamera(){
    timer->stop();
    camera->close();

}
void Controller::openSeekFace(){

}
void Controller::openVideo(QString videoName){
        qDebug()<<"controller::openvideo";
        camera->openVideo(videoName.toStdString());

        timer->start(150);

}
void Controller::chooseImg(){

//emit transImg(img);

}
void Controller::imgProcess(cv::Mat& img, long frameNum){
    seetaFace->faceDetect(img);
    cv::Mat cloneImg=img.clone();
    Worker *worker=new Worker(cloneImg,srcFeat,0.5,frameNum,seetaFace,logger);
    QObject::connect(worker,SIGNAL(hasFoundFace(float,long)),this,SLOT(foundFace(float,long)));
    threadPool->start(worker);
    //this->seetaFace->SeekFace(this->srcFeat,img);
    emit transImg(img);

}
void Controller::seekFace(cv::Mat &img){
//this->seetaFace->SeekFace(img);
}

void Controller::test(){
//camera->openVideo("F:/test.mp4");
//timer->start(50);


}
void Controller::setSrcImg(cv::Mat img){
    srcFaceImg=img;
    seetaFace->GetFeature(img,srcFeat);
}
void Controller::foundFace(float result, long frameNum){
    QString totalframeNum=QString::number(this->camera->getFrameCount(),10);

    long _sec=static_cast<long>(frameNum/this->camera->getVideoRate());
    long second=_sec%60;
    long _minute=(_sec-second)/60;
    long minute=_minute%60;
    long hour=(_minute-minute)/60;

    QString second_str=QString::number(second,10);
    QString minute_str=QString::number(minute,10);
    QString hour_str=QString::number(hour,10);
    QString time=hour_str+":"+minute_str+":"+second_str;


    QString re=QString("%1").arg(result);
    QString num=QString::number(frameNum,10);
    QString info="find at "+time+"("+num+"/"+totalframeNum+") frame and similarity is "+re;
    emit sendFaceInfo(info);
}
