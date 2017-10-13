#include "sensorview.h"
#include "ui_sensorview.h"
#include "g.h"
#include <nbasetoastr.h>

SensorView::SensorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorView)
{
    ui->setupUi(this);
}

SensorView::~SensorView()
{
    delete ui;
}

void SensorView::Update(int id)
{
    switch (id) {
    case ORD1_XSENS:
        ui->ld_roll->setText(QString::number(g::roll));
        ui->ld_pitch->setText(QString::number(g::pitch));
        ui->ld_yaw->setText(QString::number(g::yaw));
        break;
    case ORD1_KELLER:
        ui->ld_press->setText(QString::number(g::pressval));
        ui->ld_temp_s->setText(QString::number(g::tempval));
        break;
    case ORD1_ALT:
        ui->ld_d_1->setText(QString::number(g::distance[0]));
        ui->ld_e_1->setText(QString::number(g::energy[0]));
        ui->ld_c_1->setText(QString::number(g::correlation[0]));
        ui->ld_d_2->setText(QString::number(g::distance[1]));
        ui->ld_e_2->setText(QString::number(g::energy[1]));
        ui->ld_c_2->setText(QString::number(g::correlation[1]));
        ui->ld_temp_w->setText(QString::number(g::watertemp));
        break;
    case ORD1_TEMP:
        ui->ld__temp_e->setText(QString::number(g::cabin_temp));
        ui->ld__temp_ew->setText(QString::number(g::wall_temp));
        break;
    case ORD1_LIGHT:
        ui->ld_light_1->setText(QString::number(g::brightness[0]));
        ui->ld_light_2->setText(QString::number(g::brightness[1]));
        break;
    default:
        break;
    }
}

void SensorView::on_temp_check_btn_clicked()
{
    char sendbuf[7];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE;
    sendbuf[2] = 3;
    sendbuf[3] = ORD1_TEMP;
    sendbuf[4] = TEMP_ORDER;
    sendbuf[5] = 0x00;
    sendbuf[6] = 0x00;

    if(g::isConnected == true){
        int n = g::tcpSocket->write(sendbuf,7);
        if(n == 7){
            NBaseToastr * msg = new NBaseToastr(this, "温度查询发送成功");
            msg->toastr();
        }
    }
    else{
        NBaseToastr * msg = new NBaseToastr(this, "错误：未先建立通讯");
        msg->toastr();
    }
}

void SensorView::on_ligh_set_btn_1_clicked()
{
    char sendbuf[7];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE;
    sendbuf[2] = 3;
    sendbuf[3] = ORD1_LIGHT;
    sendbuf[4] = LIGHT_SET1;
    sendbuf[5] = ui->ld_light_1->text().toInt();
    sendbuf[6] = 0x00;

    if(g::isConnected == true){
        int n = g::tcpSocket->write(sendbuf,7);
        if(n == 7){
            NBaseToastr * msg = new NBaseToastr(this, "灯光亮度发送成功");
            msg->toastr();
        }
    }
    else{
        NBaseToastr * msg = new NBaseToastr(this, "错误：未先建立通讯");
        msg->toastr();
    }
}

void SensorView::on_ligh_set_btn_2_clicked()
{
    char sendbuf[7];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE;
    sendbuf[2] = 3;
    sendbuf[3] = ORD1_LIGHT;
    sendbuf[4] = LIGHT_SET2;
    sendbuf[5] = ui->ld_light_2->text().toInt();
    sendbuf[6] = 0x00;

    if(g::isConnected == true){
        int n = g::tcpSocket->write(sendbuf,7);
        if(n == 7){
            NBaseToastr * msg = new NBaseToastr(this, "灯光亮度发送成功");
            msg->toastr();
        }
    }
    else{
        NBaseToastr * msg = new NBaseToastr(this, "错误：未先建立通讯");
        msg->toastr();
    }
}
