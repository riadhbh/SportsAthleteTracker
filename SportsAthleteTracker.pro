QT       += core gui
QT       += quick
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    athletetrackerview.cpp \
    videoprocessor.cpp

HEADERS += \
    athletetrackerview.h \
    constants.h \
    videoprocessor.h

RESOURCES += athletetrackerview.qrc

INCLUDEPATH += "D:/opencv/build/install/include" \
               "D:/darknet-master\include"

LIBS += -L"D:/opencv/build/install/x64/mingw/bin" \
libopencv_aruco401 \
libopencv_bgsegm401 \
libopencv_bioinspired401 \
libopencv_calib3d401 \
libopencv_ccalib401 \
libopencv_core401 \
libopencv_cvv401 \
libopencv_datasets401 \
libopencv_dnn401 \
libopencv_dnn_objdetect401 \
libopencv_dpm401 \
libopencv_face401 \
libopencv_features2d401 \
libopencv_flann401 \
libopencv_fuzzy401 \
libopencv_gapi401 \
libopencv_hfs401 \
libopencv_highgui401 \
libopencv_imgcodecs401 \
libopencv_imgproc401 \
libopencv_img_hash401 \
libopencv_line_descriptor401 \
libopencv_ml401 \
libopencv_objdetect401 \
libopencv_optflow401 \
libopencv_phase_unwrapping401 \
libopencv_photo401 \
libopencv_plot401 \
libopencv_reg401 \
libopencv_rgbd401 \
libopencv_saliency401 \
libopencv_shape401 \
libopencv_stereo401 \
libopencv_stitching401 \
libopencv_structured_light401 \
libopencv_superres401 \
libopencv_surface_matching401 \
libopencv_text401 \
libopencv_tracking401 \
libopencv_video401 \
libopencv_videoio401 \
libopencv_videostab401 \
libopencv_xfeatures2d401 \
libopencv_ximgproc401 \
libopencv_xobjdetect401 \
libopencv_xphoto401 \
libopencv_ffmpeg401_64 \
-L"D:/darknet-master/build" \
libdarknet
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
