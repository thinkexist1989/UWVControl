#include "tcpview.h"
#include "ui_tcpview.h"
#include <QLayout>
#include <g.h>
#include <iostream>
#include <QDebug>
#include <nbasetoastr.h>

int micnt, mccnt;
int picnt, pccnt;
int xcnt, kcnt, acnt;
int ticnt, tccnt;
int licnt, lccnt;

TcpView::TcpView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpView),
    micnt(0), mccnt(0), picnt(0), pccnt(0), xcnt(0), kcnt(0), acnt(0), ticnt(0), tccnt(0), licnt(0), lccnt(0),isConnected(false)
{
    ui->setupUi(this);

    ui->uwv_ip->setText(SERVER_IP);
    ui->uwv_port->setText(QString::number(SERVER_PORT));
    ui->local_ip->setText(LOCAL_IP);
}

TcpView::~TcpView()
{
    delete ui;
}

void TcpView::FrameAnalysis(char* recvbuf, int n)
{
    if(((unsigned char)recvbuf[0] == 0xFF) && ((unsigned char)recvbuf[1] == 0xFE)) { //frame header
             switch ((unsigned char)recvbuf[3]) {
             case ORD1_MOTOR:
                 switch ((unsigned char)recvbuf[4]) {
                 case MOTOR_INFO:
                     for(int i = 0; i<8 ;i++){
                          memcpy(&g::motorvec[i].speed,&recvbuf[5+i*4],2);
                          g::motorvec[i].speed *= 3;
                          memcpy(&g::motorvec[i].current, &recvbuf[7+i*4],2);
                          g::motorvec[i].current *= 13.8;
                     }
                     //std::cout <<"Recieved New Motor Info" <<std::endl;
                     micnt++;
                     ui->ld_mi_cnt->setText(QString::number(micnt));
                     UpdateUI(ui->ld_mi_w_1,&recvbuf[3],1);
                     UpdateUI(ui->ld_mi_w_2,&recvbuf[4],1);
                     UpdateUI(ui->ld_mi_d,  &recvbuf[5],n-6);

                     emit recievedmotorinfo();
                     break;
                 case MOTOR_ORDER:
                     for(int i = 0; i<8 ;i++){                   
                          memcpy(&g::motorvec[i].pwm,&recvbuf[5+i*4],4);
                     }
                    // std::cout <<"Recieved New Motor Order" <<std::endl;
                     break;
                 default:
                     break;
                 }
                 break;
             case ORD1_PLATFORM:
                 switch ((unsigned char)recvbuf[4]) {
                 case PLATFORM_INFO:
                    memcpy(&g::CurrentPosition1, &recvbuf[5], 4);
                    memcpy(&g::CurrentPosition2, &recvbuf[9], 4);
                    memcpy(&g::CurrentPosition3, &recvbuf[13],4);
                  //  std::cout <<"Recieved New Platform Info" <<std::endl;
                    picnt++;
                    ui->ld_pi_cnt->setText(QString::number(picnt));
                    UpdateUI(ui->ld_pi_w_1,&recvbuf[3],1);
                    UpdateUI(ui->ld_pi_w_2,&recvbuf[4],1);
                    UpdateUI(ui->ld_pi_d,&recvbuf[5],n-6);

                    emit recievedplatforminfo();
                    break;
                 case PLATFORM_POS_ORDER:
                     //std::cout <<"Recieved New Platform Order" <<std::endl;
                     break;
                 case PLATFORM_SET_XYSPEED:
                     break;
                 case PLATFORM_SET_ZSPEED:
                     break;
                 case PLATFORM_SET_WSPEED:
                     break;
                 case PLATFORM_SET_XZERO:
                     break;
                 case PLATFORM_SET_YZERO:
                     break;
                 case PLATFORM_SET_ZZERO:
                     break;
                 default:
                     break;
                 }
                 break;
             case ORD1_XSENS:
                 switch ((unsigned char)recvbuf[4]) {
                 case XSENS_DATA:
                     //store the previous data to g::x_pre
                     g::roll_pre = g::roll;
                     g::pitch_pre = g::pitch;
                     g::yaw_pre = g::yaw;

                     memcpy(&g::roll, &recvbuf[5], 4);
                     memcpy(&g::pitch, &recvbuf[9], 4);
                     memcpy(&g::yaw, &recvbuf[13], 4);
                     //fit to the underwater reference framework
                     g::pitch = -g::pitch;
                     g::yaw   = -g::yaw;

                     g::droll_pre = g::droll;
                     g::dpitch_pre = g::dpitch;
                     g::dyaw_pre = g::dyaw;

                     g::droll = (g::roll - g::roll_pre)/XSENS_STEP;
                     g::dpitch = (g::pitch - g::pitch_pre)/XSENS_STEP;
                     g::dyaw = (g::yaw - g::yaw_pre)/XSENS_STEP;

                    // std::cout <<"Recieved New Xsens Info" <<std::endl;
                     xcnt++;
                     ui->ld_x_cnt->setText(QString::number(xcnt));
                     UpdateUI(ui->ld_x_w_1,&recvbuf[3],1);
                     UpdateUI(ui->ld_x_w_2,&recvbuf[4],1);
                     UpdateUI(ui->ld_x_d,&recvbuf[5],n-6);

                     emit recievedsensorinfo(ORD1_XSENS);
                     break;
                 default:
                     break;
                 }
                 break;
             case ORD1_KELLER:
                 switch ((unsigned char)recvbuf[4]) {
                 case KELLER_DATA:
                     memcpy(&g::pressval, &recvbuf[5], 4);
                     memcpy(&g::tempval, &recvbuf[9], 4);

                     g::deep_pre = g::deep;

                     g::deep = 1000*10*g::pressval;
                     g::z = 1000*10*g::pressval;

                     g::ddeep_pre = g::ddeep;

                     g::ddeep = (g::deep - g::deep_pre)/KELLER_STEP;


                     kcnt++;
                     ui->ld_k_cnt->setText(QString::number(kcnt));
                     UpdateUI(ui->ld_k_w_1,&recvbuf[3],1);
                     UpdateUI(ui->ld_k_w_2,&recvbuf[4],1);
                     UpdateUI(ui->ld_k_d,&recvbuf[5],n-6);

                     emit recievedsensorinfo(ORD1_KELLER);
                     break;
                 default:
                     break;
                 }
                 break;
             case ORD1_ALT:
                 switch ((unsigned char)recvbuf[4]) {
                 case ALT_DATA:
                     for(int i = 0; i< 2 ; i++){
                         memcpy(&g::distance[i],    &recvbuf[5 + 12*i], 4);
                         memcpy(&g::energy[i],      &recvbuf[9 + 12*i], 4);
                         memcpy(&g::correlation[i], &recvbuf[13 + 12*i], 4);
                     }
                     memcpy(&g::watertemp,    &recvbuf[29], 4);

                     acnt++;
                     ui->ld_a_cnt->setText(QString::number(acnt));
                    // std::cout << "ALT DATA!" << std::endl;
                     UpdateUI(ui->ld_a_w_1,&recvbuf[3],1);
                     UpdateUI(ui->ld_a_w_2,&recvbuf[4],1);
                     UpdateUI(ui->ld_a_d,&recvbuf[5],n-6);

                     emit recievedsensorinfo(ORD1_ALT);
                     break;
                 default:
                     break;
                 }
                 break;
             case ORD1_TEMP:
                 switch ((unsigned char)recvbuf[4]) {
                 case TEMP_DATA:
                     memcpy(&g::cabin_temp, &recvbuf[5], 4);
                     memcpy(&g::wall_temp, &recvbuf[9], 4);

                     ticnt++;
                     ui->ld_ti_cnt->setText(QString::number(ticnt));
                     UpdateUI(ui->ld_ti_w_1,&recvbuf[3],1);
                     UpdateUI(ui->ld_ti_w_2,&recvbuf[4],1);
                     UpdateUI(ui->ld_ti_d,&recvbuf[5],n-6);

                     emit recievedsensorinfo(ORD1_TEMP);
                     break;
                 case TEMP_ORDER:
                     break;
                 default:
                     break;
                 }
                 break;
             case ORD1_LIGHT:
                 switch ((unsigned char)recvbuf[4]) {
                 case LIGHT_DATA1:
                     g::brightness[0] = (short)recvbuf[5];
                    // std::cout <<"Recieved New Light Info" <<std::endl;
                     licnt++;
                     ui->ld_li_cnt->setText(QString::number(licnt));
                     UpdateUI(ui->ld_li_w_1,&recvbuf[3],1);
                     UpdateUI(ui->ld_li_w_2,&recvbuf[4],1);
                     UpdateUI(ui->ld_li_d,&recvbuf[5],n-6);

                     emit recievedsensorinfo(ORD1_LIGHT);
                     break;
                 case LIGHT_SET1:
                     break;
                 case LIGHT_DATA2:
                     g::brightness[1] = (short)recvbuf[5];
                   //  std::cout <<"Recieved New Light Info" <<std::endl;
                     licnt++;
                     ui->ld_li_cnt->setText(QString::number(licnt));
                     UpdateUI(ui->ld_li_w_1,&recvbuf[3],1);
                     UpdateUI(ui->ld_li_w_2,&recvbuf[4],1);
                     UpdateUI(ui->ld_li_d,&recvbuf[5],n-6);

                     emit recievedsensorinfo(ORD1_LIGHT);
                     break;
                 case LIGHT_SET2:
                     break;
                 default:
                     break;
                 }
             default:
                 break;
             }
         }
}

void TcpView::RecvData()
{
    int size = g::tcpSocket->bytesAvailable();
    char *p = new char[size];
    int n = g::tcpSocket->read(p,size);
  //  std::cout << "hello   "<< n  << "  " << size << std::endl;
    int i = 0;
    while(i != n-2){
        if(((unsigned char)p[i] == 0xff)&&((unsigned char)p[i+1] == 0xfe)){
            int len = p[i+2];
            if((unsigned char)p[i+3+len] == 0x00){
                FrameAnalysis(&p[i],len+4);
            }
        }
        i++;
    }

    delete p;
}

void TcpView::DisplayError(QAbstractSocket::SocketError)
{
    qDebug()<<g::tcpSocket->errorString();
    g::tcpSocket->close();
    isConnected = false;
}

void TcpView::UpdateUI(QLineEdit *pld, char *buf, int n)
{
    int i = 0;
    QString strHex;
    while(i < n){
       QString str = QString("0x%1").arg(buf[i]&0xFF,2,16,QLatin1Char('0'));
       strHex += str + " ";
       i++;
    }
    pld->setText(strHex);
}


void TcpView::on_Btn_Conn_clicked()
{
    if(g::isConnected == false){
       // g::tcpSocket->abort();
        g::tcpSocket->connectToHost(SERVER_IP , SERVER_PORT);
        if(g::tcpSocket->isOpen()){
            g::isConnected = true;
            NBaseToastr *msg = new NBaseToastr(this,"连接成功");
            msg->toastr();
            connect(g::tcpSocket, SIGNAL(readyRead()), this, SLOT(RecvData()));
            connect(g::tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(DisplayError(QAbstractSocket::SocketError)));

            ui->Btn_Conn->setText("Disconnect");
            ui->Btn_Conn->setStyleSheet("color:white;background:blue");
        }
        else{
             NBaseToastr *msg = new NBaseToastr(this,"连接失败");
             msg->toastr();
        }


    }
    else{
        g::tcpSocket->disconnectFromHost();
        g::tcpSocket->close();
        g::isConnected = false;

        ui->Btn_Conn->setText("Connect");
        ui->Btn_Conn->setStyleSheet("color:white;background:red");

        NBaseToastr *msg = new NBaseToastr(this,"断开成功");
        msg->toastr();
    }
}
