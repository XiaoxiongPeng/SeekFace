#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T14:58:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeekFace
TEMPLATE = app
RC_ICONS=myapp.ico

CONFIG+=c++11
SOURCES += main.cpp\
        mainwindow.cpp \
    myutil.cpp \
    seetaface.cpp \
    camera.cpp \
    controller.cpp

HEADERS  += mainwindow.h \
    myutil.h \
    seetaface.h \
    camera.h \
    controller.h \
    worker.h \
    logger.h

FORMS    += mainwindow.ui

INCLUDEPATH += D:\opencv2411\opencv\build\include\opencv2 \
        D:\opencv2411\opencv\build\include\opencv \
        D:\opencv2411\opencv\build\include \
        E:\SeetaFaceEngine-master-for-dll\FaceIdentification\include \
        E:\SeetaFaceEngine-master-for-dll\FaceDetection\include \
        E:\SeetaFaceEngine-master-for-dll\FaceAlignment\include

CONFIG(release,debug|release){
    LIBS += -LD:\opencv2411\opencv\build\x64\vc12\lib \
            -lopencv_calib3d2411 \
            -lopencv_contrib2411 \
            -lopencv_core2411 \
            -lopencv_features2d2411 \
            -lopencv_flann2411 \
            -lopencv_gpu2411 \
            -lopencv_highgui2411 \
            -lopencv_imgproc2411 \
            -lopencv_legacy2411 \
            -lopencv_ml2411d \
            -lopencv_objdetect2411 \
            -lopencv_ts2411 \
            -lopencv_video2411 \
            -lopencv_nonfree2411 \
            -lopencv_ocl2411 \
            -lopencv_photo2411 \
            -lopencv_stitching2411 \
            -lopencv_superres2411 \
            -lopencv_videostab2411 \
            -LE:\seetafacedllx64 \
            -lFaceDetection \
            -lFaceAlignment \
            -lIdentification
}
CONFIG(debug,debug|release){
    LIBS += -LE:\seetafacedllx64 \
            -lFaceDetection \
            -lFaceAlignment \
            -lIdentification \
            -LD:\opencv2411\opencv\build\x64\vc12\lib \
            -lopencv_calib3d2411d \
            -lopencv_contrib2411d \
            -lopencv_core2411d \
            -lopencv_features2d2411d \
            -lopencv_flann2411d \
            -lopencv_gpu2411d \
            -lopencv_highgui2411d \
            -lopencv_imgproc2411d \
            -lopencv_legacy2411d \
            -lopencv_ml2411d \
            -lopencv_objdetect2411d \
            -lopencv_ts2411d \
            -lopencv_video2411d \
            -lopencv_nonfree2411d \
            -lopencv_ocl2411d \
            -lopencv_photo2411d \
            -lopencv_stitching2411d \
            -lopencv_superres2411d \
            -lopencv_videostab2411d

}

RESOURCES += \
    seek_face_resource.qrc

