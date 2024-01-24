#include "videoprocessor.h"
#include "constants.h"

VideoProcessor::VideoProcessor(QObject *parent) : QThread(parent) {}


/*
 *Video processing logic
 */
void VideoProcessor::processVideo()
{

    std::ofstream outputFile(OUT_LOG_FILE);
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Couldn't create the output text file." << std::endl;
        emit processingFailed();
        return;
    }

    try
    {
        cv::VideoCapture inputVideo(videoFilePath);
        if (!inputVideo.isOpened())
        {
            std::cerr << "Error: Couldn't open the input video file." << std::endl;
            emit processingFailed();
            return;
        }

        cv::Size frameSize(int(inputVideo.get(cv::CAP_PROP_FRAME_WIDTH)), int(inputVideo.get(cv::CAP_PROP_FRAME_HEIGHT)));

        cv::VideoWriter outputVideo(OUT_VIDEO_FILE, CV_VIDEO_CODEC, CV_VIDEO_OUT_FPS, frameSize);

        if (!outputVideo.isOpened())
        {
            std::cerr << "Error: Couldn't create the output video file." << std::endl;
            emit processingFailed();
            return;
        }

        cv::Mat frame;
        cv::Rect2d trackingBox;

        long long unsigned int nbtotalFrames = static_cast<long long unsigned int>(inputVideo.get(cv::CAP_PROP_FRAME_COUNT));
        long long unsigned int currentFrameIndex =0;
        float progress = 0;

        // Load YOLO model and weights for person detection
        cv::dnn::Net yoloNet = cv::dnn::readNet(YOLO_WEIGHTS_FILE, YOLO_CONFIG_FILE);
        yoloNet.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        yoloNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

        while (inputVideo.read(frame))
        {
            currentFrameIndex++;
            progress = (static_cast<float>(currentFrameIndex) * 100 / nbtotalFrames);
            outputFile<< "Frame: " << currentFrameIndex << " : "
                      << "Progress = " << progress << "%" <<std::endl
                      << "PrevioustrackingBox = ( "<< trackingBox.x<<" , "<< trackingBox.y<<" ) ; "
                      << "Size: (" << trackingBox.width << ", " << trackingBox.height << " ) "<< std::endl;


            // Run YOLO inference on the frame
            std::vector<PersonDetection> detections = detectPersonsYOLO(yoloNet, frame);
            trackingBox = findNearestRectangle(detections, trackingBox);
            rectangle(frame, trackingBox, CV_GREEN_SCALAR, CV_RECTANGLE_THICKNESS);

            outputVideo.write(frame);

            //cv::imshow("Tracked Athlete",frame);

            emit updateProgress(progress);
            std::cout<<"Progress = "<< progress <<" % "<<std::endl;
            // Write the tracking information to the output log file
            outputFile<< "NextTrackingBox = (" << trackingBox.x << ", " << trackingBox.y << " ) ; "
                      << "Size: (" << trackingBox.width << ", " << trackingBox.height << " ) "<<std::endl
                      << std::setfill('-')<<std::setw(100)<< "\n" <<std::endl;
        }
        inputVideo.release();
        outputVideo.release();
        outputFile.close(); // Close the output text file

    }
    catch (std::exception &e)
    {
        std::cerr  << e.what() << std::endl;
        outputFile << "Exception caught: "<< e.what() << std::endl;
        outputFile.close(); // Close the output text file
        emit processingFailed();
    }

    emit processingComplete();
    std::cout << "Video processing is finished!" << std::endl;
}


/*
 * Calculates distances between rectangles
 */
double VideoProcessor::calculateDifference(const cv::Rect2d& rect1, const cv::Rect2d& rect2)
{
   try
    {
        double xDiff = std::abs(rect1.x - rect2.x);
        double yDiff = std::abs(rect1.y - rect2.y);

        // Calculate the Euclidean distance between the two rectangles.
        return std::sqrt(xDiff * xDiff + yDiff * yDiff);
    }
    catch (std::exception & e)
    {
        std::cerr<<e.what()<<std::endl;
        emit processingFailed();
        return std::numeric_limits<double>::min();;
    }
}


/*
 * Remove the duplicates rectangeles that are near to each other,
 * and keep the rectangles that have the best confidence score
 */
void VideoProcessor::filterRectangles(std::vector<PersonDetection>& rectangles, double threshold)
{
    try
    {
        std::vector<PersonDetection> filteredRectangles;

        for (size_t i = 0; i < rectangles.size(); ++i)
        {
            bool keepRectangle = true;

            for (size_t j = i + 1; j < rectangles.size(); ++j)
            {
                if (rectangles[i].confidence<=rectangles[j].confidence &&
                        calculateDifference(rectangles[i].rect, rectangles[j].rect) <= threshold )
                {
                    // Rectangles are too similar, so don't keep the second one.
                    keepRectangle = false;
                    break;
                }
            }

            if (keepRectangle)
            {
                filteredRectangles.push_back(rectangles[i]);
            }
        }

        rectangles = filteredRectangles; // Update the original vector with filtered rectangles.
    }
    catch (std::exception & e)
    {
        std::cerr<<e.what()<<std::endl;
        emit processingFailed();
        throw;
    }
}

/*
 * Used to find the nearset rectangle to the targetRect (the previous detected rectangle)
 * with the best confidence score
 */
cv::Rect2d VideoProcessor::findNearestRectangle(const std::vector<PersonDetection>& rectVector, const cv::Rect2d& targetRect)
{
    try
    {
        if (rectVector.empty())
        {
            return cv::Rect2d(); // Return an empty Rect2d (0, 0, 0, 0)
        }

        double minDifference = calculateDifference(rectVector[0].rect, targetRect);
        float maxConfidence = rectVector[0].confidence;
        cv::Rect2d nearestRect = rectVector[0].rect;

        for (const auto& detect : rectVector)
        {
            double difference = calculateDifference(detect.rect, targetRect);
            if (difference < minDifference && detect.confidence >= maxConfidence)
            {
                minDifference = difference;
                nearestRect = detect.rect;
                maxConfidence = detect.confidence;
            }
        }

        return nearestRect;
    }
    catch (std::exception & e)
    {
        std::cerr<<e.what()<<std::endl;
        emit processingFailed();
        return cv::Rect2d();//empty
    }
}

/*
 * Using the YOLO (You Only Look Once) algorithm to detect persons in video frames
 */
std::vector<PersonDetection> VideoProcessor::detectPersonsYOLO(cv::dnn::Net &yoloNet, const cv::Mat &frame)
{
    // Use YOLO to perform person detection on the frame
    std::vector<PersonDetection> detections;
    try
    {


        // Run YOLO inference on the frame
        cv::Mat blob = cv::dnn::blobFromImage(frame, CV_SCALE_FACTOR, YOLO_INPUT_IMAGE_SIZE, CV_BLACK_SCALAR, CV_SWAP_TO_RGB, CV_CROP);

        yoloNet.setInput(blob);
        std::vector<cv::Mat> yoloOutputs;
        yoloNet.forward(yoloOutputs, yoloNet.getUnconnectedOutLayersNames());

       // Process the detection results for persons
        for (const auto &output : yoloOutputs)
        {
            for (int i = 0; i < output.rows; ++i)
            {
                cv::Mat detection = output.row(i);
                float confidence = detection.at<float>(YOLO_CONFIDENCE_INDEX);
                int classID = static_cast<int>(detection.at<float>(YOLO_CLASS_ID_INDEX)); // Class ID is at index 5
                if (confidence > YOLO_CONFIDENCE_THRESHOLD && classID == YOLO_PERSON_CLASS_ID)
                {
                    cv::Rect2d rect;
                    rect.x = detection.at<float>(YOLO_X_INDEX) * frame.cols;
                    rect.y = detection.at<float>(YOLO_Y_INDEX) * frame.rows;
                    rect.width = detection.at<float>(YOLO_WIDTH_INDEX) * frame.cols;
                    rect.height = detection.at<float>(YOLO_HEIGHT_INDEX) * frame.rows;

                    rect.x -= (rect.width) / 2;
                    rect.y -= (rect.height) / 2;

                    detections.push_back({rect, confidence});
                }
            }
        }

        filterRectangles(detections, RECTANGLES_MIN_DISTANCE);
        return detections;
    }
    catch (std::exception & e)
    {
        std::cerr<<e.what()<<std::endl;
        emit processingFailed();
        return detections;
    }
}



void VideoProcessor::setVideoInputVideoFilePath(const std::string& videoFilePath)
{
    this->videoFilePath = videoFilePath;
}
