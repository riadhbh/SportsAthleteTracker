#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <opencv2/opencv.hpp>
/*=================================================App Constants==================================================*/
constexpr int         RECTANGLES_MIN_DISTANCE   = 30;
const     std::string OUT_VIDEO_FILE            = "processed_video.mp4";
const     std::string OUT_LOG_FILE              = "processing.log";
/*================================================OpenCV Constants================================================*/
constexpr int         CV_VIDEO_OUT_FPS          = 30;
constexpr int         CV_RECTANGLE_THICKNESS    = 2;
const     int         CV_VIDEO_CODEC            = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
const     cv::Scalar  CV_BLACK_SCALAR           = cv::Scalar(0, 0, 0);//The mean subtraction value
const     cv::Scalar  CV_GREEN_SCALAR           = cv::Scalar(0, 255, 0);
constexpr double      CV_SCALE_FACTOR           = 1.0/255.0;
constexpr bool        CV_SWAP_TO_RGB            = true;
constexpr bool        CV_CROP                   = false;
/*=================================================Yolo Constants=================================================*/
const     cv::Size    YOLO_INPUT_IMAGE_SIZE     = cv::Size(416, 416);
const     std::string YOLO_CONFIG_FILE          = "yolov3.cfg";
const     std::string YOLO_WEIGHTS_FILE         = "yolov3.weights";
constexpr float       YOLO_CONFIDENCE_THRESHOLD = 0.5;
constexpr int         YOLO_PERSON_CLASS_ID      = 0;
constexpr int         YOLO_X_INDEX              = 0;
constexpr int         YOLO_Y_INDEX              = 1;
constexpr int         YOLO_WIDTH_INDEX          = 2;
constexpr int         YOLO_HEIGHT_INDEX         = 3;
constexpr int         YOLO_CONFIDENCE_INDEX     = 4;
constexpr int         YOLO_CLASS_ID_INDEX       = 5;
/*================================================================================================================*/
#endif // CONSTANTS_H
