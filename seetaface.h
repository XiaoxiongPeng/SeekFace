#ifndef SEETAFACE_H
#define SEETAFACE_H
#include <string>
#include<iostream>
using std::string;
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "face_detection.h"
#include "face_alignment.h"
#include "face_identification.h"

struct Feature{
    string name;
    float* data;
};

class Detector : public seeta::FaceDetection {
public:
    Detector(const char * model_name);
};

class SeetaFace {
public:
    SeetaFace();
    Detector* detector;
    seeta::FaceAlignment* point_detector;
    seeta::FaceIdentification* face_recognizer;
    bool GetFeature(string filename, float* feat);
    bool GetFeature(cv::Mat img, float* feat);
    float* NewFeatureBuffer();
    float FeatureCompare(float* feat1, float* feat2);
    int GetFeatureDims();
    void faceDetect(cv::Mat img);
    bool GetFeaturesAndFaceInfos(cv::Mat img, std::vector<float *> &features, std::vector<seeta::FaceInfo> &faceInfos);
    void PointOutFace(cv::Mat img, seeta::FaceInfo faceInfo, float similarity);
    void PointOutFace(cv::Mat img, seeta::FaceInfo faceInfo);
    void SeekFace(float* srcFeat,cv::Mat img);
};


#endif // SEETAFACE_H
