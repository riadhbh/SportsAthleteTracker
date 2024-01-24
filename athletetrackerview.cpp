#include "athletetrackerview.h"
#include "ui_athletetrackerView.h"
#include "constants.h"
#include <QFileDialog>
#include <QMessageBox>

AthleteTrackerView::AthleteTrackerView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AthleteTrackerView)
{
    ui->setupUi(this);

    ui->PlayBtn->setDisabled(true);
    ui->OpenLogBtn->setDisabled(true);

    // Connect the button's clicked signal to the slot
    connect(ui->BrowseBtn, &QPushButton::clicked, this, &AthleteTrackerView::browseVideoFile);
    connect(ui->ProcessBtn, &QPushButton::clicked, this, &AthleteTrackerView::onProcessBtnClick);

    connect(ui->PlayBtn, &QPushButton::clicked, this, &AthleteTrackerView::onPlayVideoBtnClicked);
    connect(ui->OpenLogBtn, &QPushButton::clicked, this, &AthleteTrackerView::onOpenLogBtnClicked);



}

AthleteTrackerView::~AthleteTrackerView()
{
    delete ui;
}


void AthleteTrackerView::browseVideoFile()
{
    selectedFile = QFileDialog::getOpenFileName(this, "Open Video File", QDir::homePath(), "Video Files (*.mp4 *.avi *.mkv);;All Files (*)");

    ui->filePathLabel->setText(selectedFile);

}



void AthleteTrackerView::onProcessBtnClick()
{
    // Disable the "Process" button while processing
    if(!selectedFile.isNull() && !selectedFile.isEmpty())
    {
        ui->ProcessBtn->setDisabled(true);

        // Create an instance of VideoProcessor
        VideoProcessor* videoProcessor = new VideoProcessor();
        // Create a QThread and move the VideoProcessor to the thread
        auto thread = new QThread;
        videoProcessor->moveToThread(thread);
        videoProcessor->setVideoInputVideoFilePath(selectedFile.toStdString());
        // Connect signals/slots for cleanup
        connect(thread, &QThread::started, videoProcessor, &VideoProcessor::processVideo);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        connect(videoProcessor, &VideoProcessor::processingComplete, this, &AthleteTrackerView::onProcessingFinished);
        connect(videoProcessor, &VideoProcessor::processingFailed, this, &AthleteTrackerView::onProcessingFailed);
        connect(videoProcessor, &VideoProcessor::updateProgress, this, &AthleteTrackerView::onProgress);

        // Start the thread
        thread->start();
    }
}



void AthleteTrackerView::onProcessingFinished()
{
    // Video processing is complete
    ui->ProcessBtn->setEnabled(true);
    ui->PlayBtn->setEnabled(true);
    ui->OpenLogBtn->setEnabled(true);
    QMessageBox::information(this, "Processing Complete", "Video processing is finished!");
}


void AthleteTrackerView::onProcessingFailed()
{
    // Video processing is failed
    ui->ProcessBtn->setEnabled(true);
    ui->PlayBtn->setEnabled(false);
    ui->OpenLogBtn->setEnabled(false);
    QMessageBox::critical(this, "Processing Complete", "Video process failed!");
}


void AthleteTrackerView::onPlayVideoBtnClicked()
{
    std::string cmd;
    #ifdef _WIN32
        cmd = ".\\";
    #else
        cmd = "./";
    #endif

    cmd+=OUT_VIDEO_FILE;

    if (std::system(cmd.c_str()) == 0)
    {
        std::cout<<OUT_VIDEO_FILE<<" Opened !"<<std::endl;
    }
    else
    {
        std::cerr<<" Unable to open "<<OUT_VIDEO_FILE<<std::endl;
    }
}

void AthleteTrackerView::onOpenLogBtnClicked()
{
    std::string cmd;
    #ifdef _WIN32
        cmd = ".\\";
    #else
        cmd = "./";
    #endif

    cmd+=OUT_LOG_FILE;

    if (std::system(cmd.c_str()) == 0)
    {
        std::cout<<OUT_LOG_FILE<<" Opened !"<<std::endl;
    }
    else
    {
        std::cerr<<" Unable to open "<<OUT_LOG_FILE<<std::endl;
    }
}

void AthleteTrackerView::onProgress(const float& progress)
{
    // Update the progress bar here
    ui->progressBar->setValue(static_cast<int>(progress));
}
