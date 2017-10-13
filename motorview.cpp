#include "motorview.h"
#include "ui_motorview.h"
#include "g.h"
#include <iostream>
#include <nbasetoastr.h>

MotorView::MotorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorView)
{
    ui->setupUi(this);
}

MotorView::~MotorView()
{
    delete ui;
}

void MotorView::Update()
{
    ui->speed_1->setValue(g::motorvec[0].speed);
    ui->speed_2->setValue(g::motorvec[1].speed);
    ui->speed_3->setValue(g::motorvec[2].speed);
    ui->speed_4->setValue(g::motorvec[3].speed);
    ui->speed_5->setValue(g::motorvec[4].speed);
    ui->speed_6->setValue(g::motorvec[5].speed);
    ui->speed_7->setValue(g::motorvec[6].speed);
    ui->speed_8->setValue(g::motorvec[7].speed);

    ui->current_1->setValue(g::motorvec[0].current);
    ui->current_2->setValue(g::motorvec[1].current);
    ui->current_3->setValue(g::motorvec[2].current);
    ui->current_4->setValue(g::motorvec[3].current);
    ui->current_5->setValue(g::motorvec[4].current);
    ui->current_6->setValue(g::motorvec[5].current);
    ui->current_7->setValue(g::motorvec[6].current);
    ui->current_8->setValue(g::motorvec[7].current);
}

void MotorView::on_SendButton_clicked()
{
    float pwm[8];
    pwm[0] = ui->pwm_1->value();
    pwm[1] = ui->pwm_2->value();
    pwm[2] = ui->pwm_3->value();
    pwm[3] = ui->pwm_4->value();
    pwm[4] = ui->pwm_5->value();
    pwm[5] = ui->pwm_6->value();
    pwm[6] = ui->pwm_7->value();
    pwm[7] = ui->pwm_8->value();
    char sendbuf[38];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE;
    sendbuf[2] = 34;
    sendbuf[3] = ORD1_MOTOR;
    sendbuf[4] = MOTOR_ORDER;
    for(int i = 0; i < 8; i++){
        memcpy(&sendbuf[5 + 4*i],&pwm[i],4);
    }
    sendbuf[37] = 0x00;

    if(g::isConnected == true){
        int n = g::tcpSocket->write(sendbuf,38);
        if(n == 38){
            NBaseToastr * msg = new NBaseToastr(this, "指令发送成功");
            msg->toastr();
        }
    }
    else{
        NBaseToastr * msg = new NBaseToastr(this, "错误：未先建立通讯");
        msg->toastr();
    }
}

void MotorView::on_pwm_1_valueChanged(double arg1)
{
    g::motorvec[0].pwm = arg1;
}

void MotorView::on_pwm_2_valueChanged(double arg1)
{
    g::motorvec[1].pwm = arg1;
}

void MotorView::on_pwm_3_valueChanged(double arg1)
{
    g::motorvec[2].pwm = arg1;
}

void MotorView::on_pwm_4_valueChanged(double arg1)
{
    g::motorvec[3].pwm = arg1;
}

void MotorView::on_pwm_5_valueChanged(double arg1)
{
    g::motorvec[4].pwm = arg1;
}

void MotorView::on_pwm_6_valueChanged(double arg1)
{
    g::motorvec[5].pwm = arg1;
}

void MotorView::on_pwm_7_valueChanged(double arg1)
{
    g::motorvec[6].pwm = arg1;
}

void MotorView::on_pwm_8_valueChanged(double arg1)
{
    g::motorvec[7].pwm = arg1;
//    std::cout << g::motorvec[7].pwm << std::endl;
}
