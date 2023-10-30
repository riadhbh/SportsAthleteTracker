#ifndef ATHLETETRACKERVIEW_H
#define ATHLETETRACKERVIEW_H

#include <QMainWindow>
#include "videoprocessor.h"
QT_BEGIN_NAMESPACE
namespace Ui { class AthleteTrackerView; }
QT_END_NAMESPACE

class AthleteTrackerView : public QMainWindow
{
    Q_OBJECT

public:
    AthleteTrackerView(QWidget *parent = nullptr);
    ~AthleteTrackerView();

private:
    void browseVideoFile();
private slots:
    void onProcessBtnClick();
    void onProcessingFinished();
    void onProcessingFailed();
    void onPlayVideoBtnClicked();
    void onOpenLogBtnClicked();
    void onProgress(const float& progress);

private:
    Ui::AthleteTrackerView *ui;
    QString selectedFile;
    VideoProcessor videoProcessor;

};
#endif // ATHLETETRACKERVIEW_H
