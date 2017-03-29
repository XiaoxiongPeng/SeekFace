#ifndef WORKER_H
#define WORKER_H
#include <QRunnable>
#include <opencv2/core/core.hpp>
#include <highgui/highgui.hpp>
#include "seetaface.h"
#include "logger.h"
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
class Worker:public QObject,public QRunnable{//:public QObject,
    Q_OBJECT
public :
    Worker(cv::Mat& img,float* srcFeat,float similarity_floor,long frameNum,SeetaFace* seetaFace,Logger* logger){
        this->img=img;
        this->seetaFace=seetaFace;
        this->srcFeat=srcFeat;
        this->logger=logger;
        this->frameNum=frameNum;
        this->similarity_floor=similarity_floor;
        this->setAutoDelete(true);
    }
    ~Worker(){
        img.release();
        qDebug()<<"worker destructor";
    }
    void run(){
       string time= QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss zzz").toStdString();
        float* feat=seetaFace->NewFeatureBuffer();
        seetaFace->GetFeature(img,feat);
        float result=seetaFace->FeatureCompare(srcFeat,feat);
        logger->log(time,"somebody","somewhere",result);

        if(result>this->similarity_floor){
         emit hasFoundFace(result,this->frameNum);
            qDebug()<<QThread::currentThreadId();
        }

    }

private:
    cv::Mat img;
    SeetaFace *seetaFace;
    float* srcFeat;
    Logger* logger;
    long frameNum;
    float similarity_floor;
signals:
    void hasFoundFace(float result,long frameNum);
};

#endif // WORKER_H
