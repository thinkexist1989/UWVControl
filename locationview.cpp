#include "locationview.h"
#include "ui_locationview.h"
#include <g.h>
#include <QPainter>

LocationView::LocationView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocationView)
{
    ui->setupUi(this);
}

LocationView::~LocationView()
{
    delete ui;
}

void LocationView::Update(int id)
{
    Vector3f loc;
    switch (id) {
    case ORD1_XSENS:
        ui->roll_info->setValue(g::roll);
        ui->pitch_info->setValue(g::pitch);
        ui->yaw_info->setValue(g::yaw);
        break;
    case ORD1_KELLER:
        ui->z_info->setValue(g::z);
        break;
    case ORD1_ALT:
        ui->front_dis->setValue(g::distance[0]);
        ui->side_dis->setValue(g::distance[1]);
        loc = m_loc.Calc_xy(g::roll,g::pitch,g::yaw,g::distance[0],g::distance[1]);
        g::x = loc(0);
        g::y = loc(1);
        break;
    default:
        break;
    }
}
