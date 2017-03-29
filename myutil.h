#ifndef MYUTIL_H
#define MYUTIL_H
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QImage>
class MyUtil{
public :
    MyUtil();
   static  QImage Mat2QImage(const cv::Mat& mat);
   static cv::Mat QImage2Mat( QImage image);
};

#endif // MYUTIL_H
