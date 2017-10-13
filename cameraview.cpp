#include "cameraview.h"
#include "ui_cameraview.h"
#include <nbasetoastr.h>
#include <g.h>

CameraView::CameraView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraView),
    m_bIsLogin1(FALSE) ,m_bIsPlay1(FALSE), m_bIsRecording1(FALSE),
    m_bIsLogin2(FALSE) ,m_bIsPlay2(FALSE), m_bIsRecording2(FALSE)
{
    ui->setupUi(this);
}

CameraView::~CameraView()
{
    delete ui;
}

void CameraView::on_CamButton2_clicked()
{
    if(!m_bIsLogin1){  //click to preview
        NET_DVR_Init();
        NET_DVR_SetConnectTime(3000, 1);
        NET_DVR_SetReconnect(10000, TRUE);

        strcpy((char*)struLoginInfo1.sDeviceAddress, "192.168.0.13");
        strcpy((char*)struLoginInfo1.sUserName, "admin");
        strcpy((char*)struLoginInfo1.sPassword,"hykj123456");
        struLoginInfo1.wPort = 8002;
        struLoginInfo1.bUseAsynLogin = 0;

        lUserID1 = NET_DVR_Login_V40(&struLoginInfo1, &struDeviceInfo1);
        if (lUserID1 < 0){
            NBaseToastr *msg = new NBaseToastr(this,"登陆失败");
            msg->toastr();
            NET_DVR_Cleanup();
            return;
        }

        NBaseToastr *msg = new NBaseToastr(this,"登陆成功");
        msg->toastr();
        ui->CamButton1->setText("Click to Logout");
        m_bIsLogin1 = TRUE;

        WId hWnd = this->ui->CamPreview2->winId();
        struPlayInfo1.hPlayWnd = (HWND)hWnd; //需要SDK 解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo1.lChannel = 1; //预览通道号
        struPlayInfo1.dwStreamType = 0; //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo1.dwLinkMode = 0; //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo1.bBlocked = 1; //0- 非阻塞取流，1- 阻塞取流
        lRealPlayHandle1 = NET_DVR_RealPlay_V40(lUserID1, &struPlayInfo1, NULL, NULL);
        m_bIsPlay1 = TRUE;
    }
    else{ //click to logout
        NET_DVR_StopRealPlay(lRealPlayHandle1);
        m_bIsPlay1 = FALSE;

        NET_DVR_Logout(lUserID1);
        NET_DVR_Cleanup();

        NBaseToastr *msg = new NBaseToastr(this,"注销成功");
        msg->toastr();
        ui->CamButton1->setText("Click to Preview");
        m_bIsLogin1 = FALSE;
    }
}

void CameraView::on_CamButton1_clicked()
{
    if(!m_bIsLogin2){  //click to preview
        NET_DVR_Init();
        NET_DVR_SetConnectTime(3000, 1);
        NET_DVR_SetReconnect(10000, TRUE);

        strcpy((char*)struLoginInfo2.sDeviceAddress, "192.168.0.12");
        strcpy((char*)struLoginInfo2.sUserName, "admin");
        strcpy((char*)struLoginInfo2.sPassword,"hykj123456");
        struLoginInfo2.wPort = 8000;
        struLoginInfo2.bUseAsynLogin = 0;

        lUserID2 = NET_DVR_Login_V40(&struLoginInfo2, &struDeviceInfo2);
        if (lUserID2 < 0){
            NBaseToastr *msg = new NBaseToastr(this,"登陆失败");
            msg->toastr();
            NET_DVR_Cleanup();
            return;
        }

        NBaseToastr *msg = new NBaseToastr(this,"登陆成功");
        msg->toastr();
        ui->CamButton2->setText("Click to Logout");
        m_bIsLogin2 = TRUE;

        WId hWnd = this->ui->CamPreview1->winId();
        struPlayInfo2.hPlayWnd = (HWND)hWnd; //需要SDK 解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo2.lChannel = 1; //预览通道号
        struPlayInfo2.dwStreamType = 0; //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo2.dwLinkMode = 0; //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo2.bBlocked = 1; //0- 非阻塞取流，1- 阻塞取流
        lRealPlayHandle2 = NET_DVR_RealPlay_V40(lUserID2, &struPlayInfo2, NULL, NULL);
        m_bIsPlay2 = TRUE;
    }
    else{ //click to logout
        NET_DVR_StopRealPlay(lRealPlayHandle2);
        m_bIsPlay2 = FALSE;

        NET_DVR_Logout(lUserID2);
        NET_DVR_Cleanup();

        NBaseToastr *msg = new NBaseToastr(this,"注销成功");
        msg->toastr();
        ui->CamButton2->setText("Click to Preview");
        m_bIsLogin2 = FALSE;
    }
}
