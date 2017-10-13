#ifndef PLATFORMVIEW_H
#define PLATFORMVIEW_H

#include <QWidget>
#include <HCNetSDK/HCNetSDK.h>
#include <vector>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <iostream>
#include "interpolation.h"
#include <pthread.h>
#include <QTimer>
struct Pos
{
  float x;
  float y;
  float z;
};


namespace Ui {
class PlatformView;
}

class PlatformView : public QWidget
{
    Q_OBJECT

public:
    explicit PlatformView(QWidget *parent = 0);
    ~PlatformView();

    interpolation Inter;
    std::vector<Pos> posvec;
    std::vector<Pos> readposvec;

    QTimer * t;

    bool bNewdata;
    bool bisfeed;
private:
    Ui::PlatformView *ui;

    LONG lUserID;
    LONG lRealPlayHandle;
    NET_DVR_USER_LOGIN_INFO struLoginInfo;
    NET_DVR_DEVICEINFO_V40 struDeviceInfo;
    NET_DVR_PREVIEWINFO struPlayInfo;

    BOOL m_bIsRecording;
    BOOL m_bIsLogin;
    BOOL m_bIsPlay;

    bool bfirst;
    int rit = 1;
    bool buttonflag = false;
//    int RecordPosNum = 0;

//    float RPos[10][3] = {{0}};

public slots:
    void Update();  //Recieved Platform Info
    void ManualStepMoving();
    void ManualSetZero(char sendbuf4);
    void SetSpeed(char buf4, int speedvalue);
    void StepInter();
private slots:
    void on_pushButton_clicked();
    void on_xminus_clicked();
    void on_xplus_clicked();
    void on_yminus_clicked();
    void on_yplus_clicked();
    void on_zminus_clicked();
    void on_zplus_clicked();
    void on_xzero_clicked();
    void on_yzero_clicked();
    void on_zzero_clicked();
    void on_set_speedxy_clicked();
    void on_set_speedz_clicked();
    void on_set_speedw_clicked();
    void on_execute_clicked();
    void on_record_pos_clicked();
    void on_save_file_clicked();
    void on_open_file_clicked();
    void on_clear_clicked();
    void on_reset_clicked();
    void on_start_point_clicked();
    void on_checkwirefeed_clicked();
};

#endif // PLATFORMVIEW_H
