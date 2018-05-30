#include "platformview.h"
#include "ui_platformview.h"
#include "g.h"
#include <nbasetoastr.h>


PlatformView::PlatformView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlatformView),
    bfirst(false),bNewdata(false),bisfeed(false),realtimerecording(false)
{
    ui->setupUi(this);

    /********手动进给快捷键定义***********/
    ui->xminus->setShortcut(Qt::Key_S);
    ui->xplus->setShortcut(Qt::Key_W);
    ui->yminus->setShortcut(Qt::Key_D);
    ui->yplus->setShortcut(Qt::Key_A);
    ui->zminus->setShortcut(Qt::Key_H);
    ui->zplus->setShortcut(Qt::Key_G);

}


PlatformView::~PlatformView()
{
    delete ui;
}

void PlatformView::Update()
{
    ui->pos_x->setValue(g::CurrentPosition1);
    ui->pos_y->setValue(g::CurrentPosition2);
    ui->pos_z->setValue(g::CurrentPosition3);

    ui->pos_x2->setValue(g::CurrentPosition1);
    ui->pos_y2->setValue(g::CurrentPosition2);
    ui->pos_z2->setValue(g::CurrentPosition3);

    if(realtimerecording){                      //start record realtime platfrom position
        Pos t;
        t.x = g::CurrentPosition1;
        t.y = g::CurrentPosition2;
        t.z = g::CurrentPosition3;
        realtimeposvec.push_back(t);
    }

    if(bfirst == false){
        bfirst = true;
        g::TargetPosition1 = g::CurrentPosition1;
        g::TargetPosition2 = g::CurrentPosition2;
        g::TargetPosition3 = g::CurrentPosition3;
    }

    bNewdata = true;
}

void PlatformView::on_pushButton_clicked()
{
    if(!m_bIsLogin){  //click to preview
        NET_DVR_Init();
        NET_DVR_SetConnectTime(3000, 1);
        NET_DVR_SetReconnect(10000, TRUE);

        strcpy((char*)struLoginInfo.sDeviceAddress, "192.168.0.13");
        strcpy((char*)struLoginInfo.sUserName, "admin");
        strcpy((char*)struLoginInfo.sPassword,"hykj123456");
        struLoginInfo.wPort = 8002;
        struLoginInfo.bUseAsynLogin = 0;

        lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
        if (lUserID < 0){
            NBaseToastr *msg = new NBaseToastr(this,"登陆失败");
            msg->toastr();
            NET_DVR_Cleanup();
            return;
        }

        NBaseToastr *msg = new NBaseToastr(this,"登陆成功");
        msg->toastr();
        ui->pushButton->setText("Click to Logout");
        m_bIsLogin = TRUE;

        WId hWnd = this->ui->CamView->winId();
        struPlayInfo.hPlayWnd = (HWND)hWnd; //需要SDK 解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo.lChannel = 1; //预览通道号
        struPlayInfo.dwStreamType = 0; //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo.dwLinkMode = 0; //0- TCP 方式，1- UDP 方式，2- 多播方式，3- RTP 方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo.bBlocked = 1; //0- 非阻塞取流，1- 阻塞取流
        lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
        m_bIsPlay = TRUE;
    }
    else{ //click to logout
        NET_DVR_StopRealPlay(lRealPlayHandle);
        m_bIsPlay = FALSE;

        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();

        NBaseToastr *msg = new NBaseToastr(this,"注销成功");
        msg->toastr();
        ui->pushButton->setText("Click to Preview");
        m_bIsLogin = FALSE;
    }
}

/***********手动步进**************/
void PlatformView::ManualStepMoving()
{
    char sendbuf[18];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE; //Preamble
    sendbuf[2] = 14; //LEN = ord1+ord2+data
    sendbuf[3] = ORD1_PLATFORM; sendbuf[4] = PLATFORM_POS_ORDER;
    memcpy(&sendbuf[5],&g::TargetPosition1,4);
    memcpy(&sendbuf[9],&g::TargetPosition2,4);
    memcpy(&sendbuf[13],&g::TargetPosition3,4);
    sendbuf[17] = 0x00; //CRC

    if(g::isConnected){
        int n = g::tcpSocket->write(sendbuf, 18);
        if(n == 18){
            NBaseToastr * msg = new NBaseToastr(this, "指令发送成功");
            msg->toastr();
        }
    }
    else{
        NBaseToastr * msg = new NBaseToastr(this, "错误：通讯未建立");
        msg->toastr();
    }
}

/***********手动置零**************/
void PlatformView::ManualSetZero(char sendbuf4)
{
    char sendbuf[7];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE; //Preamble
    sendbuf[2] = 3; //LEN = ord1+ord2+data
    sendbuf[3] = ORD1_PLATFORM; sendbuf[4] = sendbuf4;
    sendbuf[5] = 0x00;
    sendbuf[6] = 0x00; //CRC

    if(g::isConnected){
        int n = g::tcpSocket->write(sendbuf, 7);
        if(n == 7){
            NBaseToastr * msg = new NBaseToastr(this, "置零成功");
            msg->toastr();
        }
    }
    else{
        NBaseToastr * msg = new NBaseToastr(this, "错误：通讯未建立");
        msg->toastr();
    }
}

/***********设置最大速度**************/
void PlatformView::SetSpeed(char buf4, int speedvalue)
{
    char sendbuf[10];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE; //Preamble
    sendbuf[2] = 6; //LEN = ord1+ord2+data
    sendbuf[3] = ORD1_PLATFORM; sendbuf[4] = buf4;
    memcpy(&sendbuf[5],&speedvalue,4);
    sendbuf[9] = 0x00; //CRC

    if(g::isConnected){
        int n = g::tcpSocket->write(sendbuf, 10);
        if(n == 10){
            NBaseToastr * msg = new NBaseToastr(this, "速度设置成功");
            msg->toastr();
        }
    }
    else{
        NBaseToastr * msg = new NBaseToastr(this, "错误：通讯未建立");
        msg->toastr();
    }
}

void PlatformView::StepInter()
{
    if(bNewdata == false)
        return;
    if(g::it == g::intervec.size()){
        std::cout << "PLAT:Compeleted!!" << std::endl;
        if(bisfeed){
            std::cout << "stop feeding!!!" << std::endl;
            ui->speedw->setValue(0);
            SetSpeed(PLATFORM_SET_WSPEED, -ui->speedw->value());
        }
        t->stop();
        t->destroyed();
        g::it=0;
        g::intervec.clear();
//        if(rit == readposvec.size()-1){
//            rit = 1;
//            t->destroyed();
//        }
//        else{
//            Inter.LinearInterpolation(readposvec[rit].x,readposvec[rit].y,readposvec[rit+1].x,readposvec[rit+1].y);
//            rit++;
//        }
        return;
    }
    g::TargetPosition1 = g::intervec[g::it].x;
    g::TargetPosition2 = g::intervec[g::it].y;

    if((abs(g::TargetPosition1 - g::CurrentPosition1) < 1.5) && (abs(g::TargetPosition2-g::CurrentPosition2) < 1.5)){
        g::it++;
//        g::TargetPosition1 = g::intervec[g::it].x;
//        g::TargetPosition2 = g::intervec[g::it].y;
    }

    char sendbuf[18];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE; //Preamble
    sendbuf[2] = 14; //LEN = ord1+ord2+data
    sendbuf[3] = ORD1_PLATFORM; sendbuf[4] = PLATFORM_POS_ORDER;
    memcpy(&sendbuf[5],&g::TargetPosition1,4);
    memcpy(&sendbuf[9],&g::TargetPosition2,4);
    memcpy(&sendbuf[13],&g::TargetPosition3,4);
    sendbuf[17] = 0x00; //CRC
    if(g::isConnected)
        int n = g::tcpSocket->write(sendbuf, 18);

    bNewdata = false;
}

void PlatformView::on_xminus_clicked()
{
    if(!bfirst)
        return;
    g::TargetPosition1 = g::CurrentPosition1 - ui->step_len->value();
    ManualStepMoving();
}

void PlatformView::on_xplus_clicked()
{
    if(!bfirst)
        return;
    g::TargetPosition1 = g::CurrentPosition1 + ui->step_len->value();
    ManualStepMoving();
}

void PlatformView::on_yminus_clicked()
{
    if(!bfirst)
        return;
    g::TargetPosition2 = g::CurrentPosition2 - ui->step_len->value();
    ManualStepMoving();
}

void PlatformView::on_yplus_clicked()
{
    if(!bfirst)
        return;
    g::TargetPosition2 = g::CurrentPosition2 + ui->step_len->value();
    ManualStepMoving();
}

void PlatformView::on_zminus_clicked()
{
    if(!bfirst)
        return;
    g::TargetPosition3 = g::CurrentPosition3 - ui->step_len->value();
    ManualStepMoving();
}

void PlatformView::on_zplus_clicked()
{
    if(!bfirst)
        return;
    g::TargetPosition3 = g::CurrentPosition3 + ui->step_len->value();
    ManualStepMoving();
}

void PlatformView::on_xzero_clicked()
{
    ManualSetZero(PLATFORM_SET_XZERO);
    g::TargetPosition1 = 0;
}

void PlatformView::on_yzero_clicked()
{
    ManualSetZero(PLATFORM_SET_YZERO);
    g::TargetPosition2 = 0;
}

void PlatformView::on_zzero_clicked()
{
   ManualSetZero(PLATFORM_SET_ZZERO);
   g::TargetPosition3 = 0;
}

void PlatformView::on_set_speedxy_clicked()
{
    SetSpeed(PLATFORM_SET_XYSPEED, ui->speedxy->value());
}

void PlatformView::on_set_speedz_clicked()
{
    SetSpeed(PLATFORM_SET_ZSPEED, ui->speedz->value());
}

void PlatformView::on_set_speedw_clicked()
{
    SetSpeed(PLATFORM_SET_WSPEED, -ui->speedw->value());
    if(g::isConnected){
        QString s = QString::number(ui->speedw->value(), 10);
        ui->speed_w->setText(s);
    }

}

void PlatformView::on_execute_clicked()
{
    if(readposvec.size() == 0){
        NBaseToastr * msg = new NBaseToastr(this, "未读入数据");
        msg->toastr();
        return;
    }
    if(buttonflag == false)
    {
        NBaseToastr * msg = new NBaseToastr(this, "请移动到起点");
        msg->toastr();
        return;
    }
    buttonflag = false;
    for(int i=0;i<readposvec.size()-1;i++){
        Inter.LinearInterpolation2(readposvec[i].x,readposvec[i].y,readposvec[i+1].x,readposvec[i+1].y);
    }

//    Inter.CircularInterpolation(0,0,20,0,10,0,1);
    t = new QTimer(this);
    connect(t,SIGNAL(timeout()),this,SLOT(StepInter()));
    t->start(500);
    if(bisfeed){
        std::cout << "feeding now!!" << std::endl;
        ui->speedw->setValue(1000);
        SetSpeed(PLATFORM_SET_WSPEED, -ui->speedw->value());
    }
}

void PlatformView::on_record_pos_clicked()
{
    Pos t;
    t.x = g::CurrentPosition1;
    t.y = g::CurrentPosition2;
    t.z = g::CurrentPosition3;
    posvec.push_back(t);
    QString qx = QString("%1").arg(t.x);
    QString qy = QString("%1").arg(t.y);
    QString qz = QString("%1").arg(t.z);
    QString qa = qx+","+qy+","+qz+"\n";
    ui->textBrowser->textCursor().insertText(qa);
    ui->textBrowser->moveCursor(QTextCursor::End);

}

void PlatformView::on_save_file_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("Save Files"),"",tr("TXT Files(*.txt);;All Files(*.*)"));
    if(!fileName.isNull()){
        int m = posvec.size();
        int i=0;
        QFile data(fileName);
        if(data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
            QTextStream out(&data);
            for(i;i<m;i++){
                out << posvec[i].x << "," << posvec[i].y << "," << posvec[i].z << "\n";
            }
        }
    }
    else{
        return;
    }
}

void PlatformView::on_open_file_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("Open Files"),"",tr("TXT Files(*.txt);;All Files(*.*)"));
    if(!fileName.isNull()){
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        QTextStream in(&file);
        ui->textBrowser->setPlainText("");
        std::vector<Pos> tempvec;
        readposvec.swap(tempvec);
        while(!in.atEnd()){
            QString line = in.readLine();
            QStringList list = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);

            Pos t;
            t.x = list[0].toFloat();
            t.y = list[1].toFloat();
            t.z = list[2].toFloat();
            readposvec.push_back(t);

            ui->textBrowser->textCursor().insertText(line);
            ui->textBrowser->textCursor().insertText("\n");
        }

    }
    else{
        return;
    }
}

void PlatformView::on_clear_clicked()
{
    ui->textBrowser->setPlainText("");
    std::vector<Pos> tempvec;
    posvec.swap(tempvec);
    readposvec.swap(tempvec);
}

void PlatformView::on_reset_clicked()
{
    g::TargetPosition1 = 18.0;
    g::TargetPosition2 = -99.0;
    ManualStepMoving();
}

void PlatformView::on_start_point_clicked()
{
    if(readposvec.size() == 0){
        NBaseToastr * msg = new NBaseToastr(this, "未读入数据");
        msg->toastr();
        return;
    }
    g::TargetPosition1 = readposvec[0].x;
    g::TargetPosition2 = readposvec[0].y;
    ManualStepMoving();
    if(g::isConnected){
        buttonflag = true;
    }

}

void PlatformView::on_checkwirefeed_clicked()
{
    if(ui->checkwirefeed->checkState() == Qt::Checked){
        bisfeed = true;
        std::cout << "check is feeding" << std::endl;
    }
    else if(ui->checkwirefeed->checkState() == Qt::Unchecked){
        bisfeed = false;
        std::cout << "check is not feeding" << std::endl;
    }
}

/***********三轴平台实时位置记录**************/
void PlatformView::on_record_realtime_pos_clicked()
{
    if(realtimerecording){
        realtimerecording = false;
        int m = realtimeposvec.size();
        if(m != 0){
            NBaseToastr * msg = new NBaseToastr(this, "请保存数据");
            msg->toastr();

            QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("Save Files"),"",tr("TXT Files(*.txt);;All Files(*.*)"));
            if(!fileName.isNull()){
                int i=0;
                QFile data(fileName);
                if(data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
                    QTextStream out(&data);
                    for(i;i<m;i++){
                        out << realtimeposvec[i].x << "," << realtimeposvec[i].y << "," << realtimeposvec[i].z << "\n";
                    }
                }
            }
            std::vector<Pos> tempvec;
            realtimeposvec.swap(tempvec);
        }
        else{
            NBaseToastr * msg = new NBaseToastr(this, "未记录到任何数据");
            msg->toastr();
        }

        ui->record_realtime_pos->setStyleSheet("background-color:rgb(0,255,0)");
        ui->record_realtime_pos->setText("REC RTPos");
    }

    else{
        realtimerecording = true;
        ui->record_realtime_pos->setStyleSheet("background-color:rgb(255,0,0)");
        ui->record_realtime_pos->setText("Stop");
        NBaseToastr * msg = new NBaseToastr(this, "开始实时记录平台位置数据");
        msg->toastr();

    }
}
