This implementation for tracking athletes in a video.

The application follows a structured design with a separation of the UI logic and video processing logic. 
Users can select a video file, process it to track athletes, and then play the processed video or view the processing log. 
The video processing involves YOLO-based person detection and tracking.

This is a brief explanation of the key components and their functionality:

1. constants.h: 
	This header file contains various constants used in the application, 
	including minimum distance between rectangles, file names, OpenCV-related settings, 
	and YOLO (You Only Look Once) configuration.

2. AthleteTrackerView: 
	This is the main user interface class derived from QMainWindow, 
	which provides the graphical user interface for the application. 
	It allows users to browse for a video file, process the video, 
	play the processed video, and open a log file.

   - browseVideoFile(): Opens a file dialog for selecting a video file.
   - onProcessBtnClick(): Initiates video processing. It creates a VideoProcessor instance and runs it in a separate thread.
   - onProcessingFinished(): Called when video processing is completed successfully.
   - onProcessingFailed(): Called when video processing encounters an error.
   - onPlayVideoBtnClicked(): Opens the processed video file for playback.
   - onOpenLogBtnClicked(): Opens the processing log file.
   - onProgress(): Updates a progress bar with the processing progress.

3. VideoProcessor: 
	This class extends QThread and is responsible for the video processing logic. 
	It processes each frame of the video, detects persons using YOLO, tracks them, 
	and writes the tracking information to an output video.

   - processVideo(): The main video processing logic, including video input and output, YOLO-based person detection, tracking, and log file writing.
   - calculateDifference(): Calculates the Euclidean distance between two rectangles.
   - filterRectangles(): Filters rectangles to remove duplicates and keep the ones with the best confidence scores.
   - findNearestRectangle(): Finds the nearest rectangle to a target rectangle with the best confidence score.
   - detectPersonsYOLO(): Uses YOLO to detect persons in video frames.
   - setVideoInputVideoFilePath(): Sets the video file path for processing (called from AthleteTrackerView class to get the input video file path ).

4. UI-related files: 
	These include "athletetrackerview.ui" and "ui_athletetrackerView.h," which define the user interface components and their layout.

5. Remark:
	In the Demo pictures attached the processed frames are displayed during processing in UI, 
	the code for this feature is removed in this version, because it's not reliable.