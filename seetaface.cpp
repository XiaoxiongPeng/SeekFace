#include "seetaface.h"

Detector::Detector(const char* model_name) : seeta::FaceDetection(model_name) {
    this->SetMinFaceSize(40);
    this->SetScoreThresh(2.f);
    this->SetImagePyramidScaleFactor(0.8f);
    this->SetWindowStep(4, 4);
}

SeetaFace::SeetaFace() {
    this->detector = new Detector("model\\seeta_fd_frontal_v1.0.bin");
    this->point_detector = new seeta::FaceAlignment("model\\seeta_fa_v1.1.bin");
    this->face_recognizer = new seeta::FaceIdentification("model\\seeta_fr_v1.0.bin");
}

float* SeetaFace::NewFeatureBuffer() {
    return new float[this->face_recognizer->feature_size()];
}

bool SeetaFace::GetFeature(string filename, float* feat) {
    //如果有多张脸，输出第一张脸,把特征放入缓冲区feat，返回true
    //如果没有脸，输出false
    //read pic greyscale
    cv::Mat src_img = cv::imread(filename, 0);//0是灰度
    seeta::ImageData src_img_data(src_img.cols, src_img.rows, src_img.channels());
    src_img_data.data = src_img.data;

    //read pic color
    cv::Mat src_img_color = cv::imread(filename, 1);//1是彩色
    seeta::ImageData src_img_data_color(src_img_color.cols, src_img_color.rows, src_img_color.channels());
    src_img_data_color.data = src_img_color.data;

    std::vector<seeta::FaceInfo> faces = this->detector->Detect(src_img_data);
    int32_t face_num = static_cast<int32_t>(faces.size());
    if (face_num == 0)
    {
        return false;
    }
    seeta::FacialLandmark points[5];
    this->point_detector->PointDetectLandmarks(src_img_data, faces[0], points);

    this->face_recognizer->ExtractFeatureWithCrop(src_img_data_color, points, feat);
    return true;
}
bool SeetaFace::GetFeature(cv::Mat img, float * feat)
{
    cv::Mat src_img;
    cv::cvtColor(img, src_img, CV_BGR2GRAY);
    seeta::ImageData src_img_data(src_img.cols, src_img.rows, src_img.channels());
    src_img_data.data = src_img.data;

    //原图
    cv::Mat src_img_color = img;
    seeta::ImageData src_img_data_color(src_img_color.cols, src_img_color.rows, src_img_color.channels());
    src_img_data_color.data = src_img_color.data;

    std::vector<seeta::FaceInfo> faces = this->detector->Detect(src_img_data);
    int32_t face_num = static_cast<int32_t>(faces.size());
    if (face_num == 0)
    {
        return false;
    }


    seeta::FacialLandmark points[5];
    this->point_detector->PointDetectLandmarks(src_img_data, faces[0], points);

    this->face_recognizer->ExtractFeatureWithCrop(src_img_data_color, points, feat);


    return true;
}

bool SeetaFace::GetFeaturesAndFaceInfos(cv::Mat img, std::vector<float*> &features, std::vector<seeta::FaceInfo> &faceInfos)
{
    cv::Mat src_img;
    cv::cvtColor(img, src_img, CV_BGR2GRAY);
    seeta::ImageData src_img_data(src_img.cols, src_img.rows, src_img.channels());
    src_img_data.data = src_img.data;

    //原图
    cv::Mat src_img_color = img;
    seeta::ImageData src_img_data_color(src_img_color.cols, src_img_color.rows, src_img_color.channels());
    src_img_data_color.data = src_img_color.data;

    faceInfos = this->detector->Detect(src_img_data);
    int32_t face_num = static_cast<int32_t>(faceInfos.size());
    if (face_num == 0)
    {
        return false;
    }

    for(int i=0;i<face_num;i++){
    seeta::FacialLandmark* points=new seeta::FacialLandmark[5];
    this->point_detector->PointDetectLandmarks(src_img_data, faceInfos[i], points);
    float* f=this->NewFeatureBuffer();
    this->face_recognizer->ExtractFeatureWithCrop(src_img_data_color, points, f);
    features.push_back(f);
    }
    return true;
}
int SeetaFace::GetFeatureDims() {
    return this->face_recognizer->feature_size();
}

float SeetaFace::FeatureCompare(float* feat1, float* feat2) {
    return this->face_recognizer->CalcSimilarity(feat1, feat2);
}

void SeetaFace::faceDetect(cv::Mat img){
     cv::Mat img_gray;
     if (img.channels() != 1)
       cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
     else
       img_gray = img;

     seeta::ImageData img_data;
     img_data.data = img_gray.data;
     img_data.width = img_gray.cols;
     img_data.height = img_gray.rows;
     img_data.num_channels = 1;


     std::vector<seeta::FaceInfo> faces = this->detector->Detect(img_data);
     cv::Rect face_rect;
     int32_t num_face = static_cast<int32_t>(faces.size());

       for (int32_t i = 0; i < num_face; i++) {
         face_rect.x = faces[i].bbox.x;
         face_rect.y = faces[i].bbox.y;
         face_rect.width = faces[i].bbox.width;
         face_rect.height = faces[i].bbox.height;

         cv::rectangle(img, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
       }
}

void SeetaFace::SeekFace(float *srcFeat, cv::Mat img){

    std::vector<float*> features;
    std::vector<seeta::FaceInfo> faceInfos;

    GetFeaturesAndFaceInfos(img,features,faceInfos);

    int32_t face_num = static_cast<int32_t>(faceInfos.size());

    if(face_num==0)return;

    seeta::FaceInfo the_face_info=faceInfos[0];
    float similarity=this->FeatureCompare(srcFeat,features[0]);

    for(int i=1;i<face_num;i++){
        float simi_tmp=this->FeatureCompare(srcFeat,features[i]);
        if(simi_tmp>similarity){
            similarity=simi_tmp;
            the_face_info=faceInfos[i];
        }
    }

  this->PointOutFace(img,the_face_info,similarity);
}

void SeetaFace::PointOutFace(cv::Mat img,seeta::FaceInfo faceInfo){
    cv::Rect face_rect;
    face_rect.x = faceInfo.bbox.x;
    face_rect.y = faceInfo.bbox.y;
    face_rect.width = faceInfo.bbox.width;
    face_rect.height = faceInfo.bbox.height;

    cv::rectangle(img, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
}
void SeetaFace::PointOutFace(cv::Mat img, seeta::FaceInfo faceInfo,float similarity){
   // CvFont font;
   // cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.5f, 1.5f, 0, 2, CV_AA);//设置显示的字体
    cv::Rect face_rect;
    face_rect.x = faceInfo.bbox.x;
    face_rect.y = faceInfo.bbox.y;
    face_rect.width = faceInfo.bbox.width;
    face_rect.height = faceInfo.bbox.height;
    std::string str=std::to_string(similarity);
    //cv::putText(img,strID, cvPoint(face_rect.x, face_rect.y-10), &font, CV_RGB(255, 0, 0));//红色字体注释
    cv::putText(img, str,cvPoint(face_rect.x, face_rect.y-10), 1,1.5,cv::Scalar( 255, 255, 255), 2);
    cv::rectangle(img, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
}
