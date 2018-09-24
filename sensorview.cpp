#include "sensorview.h"
#include "ui_sensorview.h"
#include <nbasetoastr.h>

SensorView::SensorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorView),
    sensordatarecording(false)
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

        if(sensordatarecording){                      //start record Xsens data
            RECXsensData t;
            t.roll = g::roll;
            t.pitch = g::pitch;
            t.yaw = g::yaw;
            xsensdatavec.push_back(t);
        }
        break;
    case ORD1_KELLER:
        ui->ld_press->setText(QString::number(g::pressval));
        ui->ld_temp_s->setText(QString::number(g::tempval));

        if(sensordatarecording){                      //start record Keller data
            float t;
            t = g::pressval;
            kellerdatavec.push_back(t);
        }
        break;
    case ORD1_ALT:
        ui->ld_d_1->setText(QString::number(g::distance[0]));
        ui->ld_e_1->setText(QString::number(g::energy[0]));
        ui->ld_c_1->setText(QString::number(g::correlation[0]));
        ui->ld_d_2->setText(QString::number(g::distance[1]));
        ui->ld_e_2->setText(QString::number(g::energy[1]));
        ui->ld_c_2->setText(QString::number(g::correlation[1]));
        ui->ld_temp_w->setText(QString::number(g::watertemp));

        if(sensordatarecording){                      //start record Altimeter data
            RECAltData t;
            t.alt1_distance = g::distance[0];
            t.alt1_energy = g::energy[0];
            t.alt1_correlation = g::correlation[0];
            t.alt2_distance = g::distance[1];
            t.alt2_energy = g::energy[1];
            t.alt2_correlation = g::correlation[1];
            altdatavec.push_back(t);
        }
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

void SensorView::on_rec_sensor_data_clicked()
{
    if(sensordatarecording){
        sensordatarecording = false;
        int m1 = xsensdatavec.size();
        int m2 = kellerdatavec.size();
        int m3 = altdatavec.size();

        //Xsens数据记录
        if(m1 != 0){
            NBaseToastr * msg = new NBaseToastr(this, "请保存Xsens数据");
            msg->toastr();

            QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("Save Xsens Data"),"",tr("TXT Files(*.txt);;All Files(*.*)"));
            if(!fileName.isNull()){
                int i;
                QFile data(fileName);
                if(data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
                    QTextStream out(&data);
                    for(i=0;i<m1;i++){
                        out << xsensdatavec[i].roll << "," << xsensdatavec[i].pitch << "," << xsensdatavec[i].yaw << "\n";
                    }
                }
            }
            std::vector<RECXsensData> tempvec;
            xsensdatavec.swap(tempvec);
        }
        else{
            NBaseToastr * msg = new NBaseToastr(this, "未记录到Xsens数据");
            msg->toastr();
        }

        //Keller数据记录
        if(m2 != 0){
            NBaseToastr * msg = new NBaseToastr(this, "请保存Keller数据");
            msg->toastr();

            QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("Save Keller Data"),"",tr("TXT Files(*.txt);;All Files(*.*)"));
            if(!fileName.isNull()){
                int i;
                QFile data(fileName);
                if(data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
                    QTextStream out(&data);
                    for(i=0;i<m2;i++){
                        out << kellerdatavec[i] << "\n";
                    }
                }
            }
            std::vector<float> tempvec;
            kellerdatavec.swap(tempvec);
        }
        else{
            NBaseToastr * msg = new NBaseToastr(this, "未记录到Keller数据");
            msg->toastr();
        }

        //Altimeter数据记录
        if(m3 != 0){
            NBaseToastr * msg = new NBaseToastr(this, "请保存Altimeter数据");
            msg->toastr();

            QString fileName = QFileDialog::getSaveFileName(this,QString::fromLocal8Bit("Save Altimeter Data"),"",tr("TXT Files(*.txt);;All Files(*.*)"));
            if(!fileName.isNull()){
                int i;
                QFile data(fileName);
                if(data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
                    QTextStream out(&data);
                    for(i=0;i<m3;i++){
                        out << altdatavec[i].alt1_distance << "," << altdatavec[i].alt1_energy << "," << altdatavec[i].alt1_correlation << "," <<
                            altdatavec[i].alt2_distance << "," << altdatavec[i].alt2_energy << "," << altdatavec[i].alt2_correlation << "\n";
                    }
                }
            }
            std::vector<RECAltData> tempvec;
            altdatavec.swap(tempvec);
        }
        else{
            NBaseToastr * msg = new NBaseToastr(this, "未记录到Altimeter数据");
            msg->toastr();
        }

        ui->rec_sensor_data->setStyleSheet("background-color:rgb(0,255,0)");
        ui->rec_sensor_data->setText("REC Sensor Data");
    }


    else{
        sensordatarecording = true;
        ui->rec_sensor_data->setStyleSheet("background-color:rgb(255,0,0)");
        ui->rec_sensor_data->setText("Stop");
        NBaseToastr * msg = new NBaseToastr(this, "记录传感器数据");
        msg->toastr();
    }
}
