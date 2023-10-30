#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H
#include<QThread>
#include<opencv2/opencv.hpp>
#include<QMainWindow>

struct PersonDetection
{
    cv::Rect2d rect;
    float confidence;     // Confidence score of the detection
};

class VideoProcessor : public QThread
{
    Q_OBJECT

private:
    std::string videoFilePath; // Member variable to store the video file path

private:

    double calculateDifference(const cv::Rect2d& rect1, const cv::Rect2d& rect2);

    void filterRectangles(std::vector<PersonDetection>& rectangles, double threshold);

    cv::Rect2d findNearestRectangle(const std::vector<PersonDetection>& rectVector, const cv::Rect2d& targetRect);

    std::vector<PersonDetection> detectPersonsYOLO(cv::dnn::Net &yoloNet, const cv::Mat &frame);

public:
    VideoProcessor(QObject *parent = nullptr);
    void processVideo();
    void setVideoInputVideoFilePath(const std::string& videoFilePath);
signals:
    void processingComplete();
    void processingFailed();
    void updateProgress(const float& progress);

};
#endif // VIDEOPROCESSOR_H
