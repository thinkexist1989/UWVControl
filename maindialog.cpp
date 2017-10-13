#include "maindialog.h"
#include "ui_maindialog.h"
#include <cameraview.h>
#include <tcpview.h>
#include <motorview.h>
#include <platformview.h>
#include <sensorview.h>
#include <controlview.h>
#include <locationview.h>
#include "nbasetoastr.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags=Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    flags |= Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);

    CameraView *camView = new CameraView(this);
    ui->tabWidget->addTab(camView,"CameraView");

    TcpView *tcpView = new TcpView(this);
    ui->tabWidget->addTab(tcpView,"TcpView");

    MotorView *motorView = new MotorView(this);
    ui->tabWidget->addTab(motorView,"MotorView");

    PlatformView *platView = new PlatformView(this);
    ui->tabWidget->addTab(platView,"PlatformView");

    SensorView *sensorView = new SensorView(this);
    ui->tabWidget->addTab(sensorView,"SensorView");

    ControlView *ctrlView = new ControlView(this);
    ui->tabWidget->addTab(ctrlView,"ControlView");

    LocationView *locView = new LocationView(this);
    ui->tabWidget->addTab(locView,"LocationView");

    connect(tcpView,SIGNAL(recievedmotorinfo()),motorView,SLOT(Update()));
    connect(tcpView,SIGNAL(recievedplatforminfo()),platView,SLOT(Update()));
    connect(tcpView,SIGNAL(recievedsensorinfo(int)),sensorView,SLOT(Update(int)));
    connect(tcpView,SIGNAL(recievedsensorinfo(int)),ctrlView,SLOT(Update(int)));
    connect(tcpView,SIGNAL(recievedsensorinfo(int)),locView,SLOT(Update(int)));

    NBaseToastr * msg = new NBaseToastr(this, "程序启动成功");
    msg->toastr();
}

MainDialog::~MainDialog()
{
    delete ui;
}
