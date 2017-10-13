#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include <QWidget>
#include "HCNetSDK/HCNetSDK.h"

namespace Ui {
class CameraView;
}

class CameraView : public QWidget
{
    Q_OBJECT

public:
    explicit CameraView(QWidget *parent = 0);
    ~CameraView();

private slots:
    void on_CamButton1_clicked();

    void on_CamButton2_clicked();

private:
    Ui::CameraView *ui;

    LONG lUserID1;
    LONG lRealPlayHandle1;
    NET_DVR_USER_LOGIN_INFO struLoginInfo1;
    NET_DVR_DEVICEINFO_V40 struDeviceInfo1;
    NET_DVR_PREVIEWINFO struPlayInfo1;

    BOOL m_bIsRecording1;
    BOOL m_bIsLogin1;
    BOOL m_bIsPlay1;

    LONG lUserID2;
    LONG lRealPlayHandle2;
    NET_DVR_USER_LOGIN_INFO struLoginInfo2;
    NET_DVR_DEVICEINFO_V40 struDeviceInfo2;
    NET_DVR_PREVIEWINFO struPlayInfo2;

    BOOL m_bIsRecording2;
    BOOL m_bIsLogin2;
    BOOL m_bIsPlay2;

    DWORD dwStop;
};

#endif // CAMERAVIEW_H
