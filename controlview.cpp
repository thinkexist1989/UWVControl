#include "controlview.h"
#include "ui_controlview.h"
#include <iostream>
#include <QKeyEvent>

#define A     0.5
#define B     0.17
#define C     0.3
#define D     0.06
#define alpha PI/6
#define PI 3.1415926

/* Fx maximum is 192.052 N
 * Fy maximum is 110 N
 * Fz maximum is 220 N
 * Tx maximum is 37.4 N·m
 * Ty maximum is 110 N·m
 * Tz maximum is 87.3 N·m
 */

#define FXMAX 192.052
#define FYMAX 110
#define FZMAX 220
#define TXMAX 37.4
#define TYMAX 110
#define TZMAX 87.3

ControlView::ControlView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlView),
    bOrienCtrl(false),bPosCtrl(false)
{
    ui->setupUi(this);

    g::motorvec[0].dir = -1;
    g::motorvec[1].dir = -1;
    g::motorvec[2].dir =  1;
    g::motorvec[3].dir =  1;
    g::motorvec[4].dir = -1;
    g::motorvec[5].dir = -1;
    g::motorvec[6].dir =  1;
    g::motorvec[7].dir =  1;

    Vector3f p1,p2,p3,p4,p5,p6,p7,p8;
    Vector3f f1,f2,f3,f4,f5,f6,f7,f8;

    //Vetor of direction
    p1 << -A, -B, -C; p2 << -A,-B, D;
    p4 << -A,  B, -C; p3 << -A, B, D;
    p6 <<  A,  B, -C; p5 <<  A, B, D;
    p7 <<  A, -B, -C; p8 <<  A,-B, D;

    //Vector of thrust
    f1 <<  0, 0, 1;     f2 <<  cos(alpha),-sin(alpha), 0;
    f4 <<  0, 0, 1;     f3 <<  cos(alpha), sin(alpha), 0;
    f6 <<  0, 0, 1;     f5 << -cos(alpha), sin(alpha), 0;
    f7 <<  0, 0, 1;     f8 << -cos(alpha),-sin(alpha), 0;

    Vector6f V1,V2,V3,V4,V5,V6,V7,V8;
    V1 << f1,p1.cross(f1); V2 << f2,p2.cross(f2); V3 << f3,p3.cross(f3); V4 << f4,p4.cross(f4);
    V5 << f5,p5.cross(f5); V6 << f6,p6.cross(f6); V7 << f7,p7.cross(f7); V8 << f8,p8.cross(f8);


//    Matrix86f temp;
//    temp <<         0,         0,    1,    -B,        A,                       0,
//            cos(PI/6),-sin(PI/6),    0,     0,        0, A*sin(PI/6)+B*cos(PI/6),
//            cos(PI/6), sin(PI/6),    0,     0,        0,-A*sin(PI/6)-B*cos(PI/6),
//                    0,         0,    1,     B,        A,                       0,
//           -cos(PI/6), sin(PI/6),    0,     0,        0, A*sin(PI/6)+B*cos(PI/6),
//                    0,         0,    1,     B,       -A,                       0,
//                    0,         0,    1,    -B,       -A,                       0,
//           -cos(PI/6),-sin(PI/6),    0,     0,        0,-A*sin(PI/6)-B*cos(PI/6);

//    g::K = temp.transpose();
    Matrix86f temp;
    temp << V1.transpose(),V2.transpose(),V3.transpose(),V4.transpose(),V5.transpose(),V6.transpose(),V7.transpose(),V8.transpose();
    g::K = temp.transpose();
    std::cout << "K is : \n" << g::K << std::endl;

    g::K_star = g::K.transpose()*(g::K*g::K.transpose()).inverse();
    std::cout << "K_star is : \n" << g::K_star << std::endl;

}

ControlView::~ControlView()
{
    delete ui;
}

void ControlView::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_8){
        if(e->isAutoRepeat()) return;
        on_forward_pressed();
    }
    else if(e->key() == Qt::Key_2){
        if(e->isAutoRepeat()) return;
        on_backward_pressed();
    }
    else if(e->key() == Qt::Key_7){
        if(e->isAutoRepeat()) return;
        on_lturn_pressed();
    }
    else if(e->key() == Qt::Key_9){
        if(e->isAutoRepeat()) return;
        on_rturn_pressed();
    }
    else if(e->key() == Qt::Key_4){
        if(e->isAutoRepeat()) return;
        on_lmove_pressed();
    }
    else if(e->key() == Qt::Key_6){
        if(e->isAutoRepeat()) return;
        on_rmove_pressed();
    }
    else if(e->key() == Qt::Key_5){
        if(e->isAutoRepeat()) return;
        on_down_pressed();
    }
    else if(e->key() == Qt::Key_0){
        if(e->isAutoRepeat()) return;
        on_up_pressed();
    }
}

void ControlView::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_8){
        if(e->isAutoRepeat()) return;
        on_forward_released();
    }
    else if(e->key() == Qt::Key_2){
        if(e->isAutoRepeat()) return;
        on_backward_released();
    }
    else if(e->key() == Qt::Key_7){
        if(e->isAutoRepeat()) return;
        on_lturn_released();
    }
    else if(e->key() == Qt::Key_9){
        if(e->isAutoRepeat()) return;
        on_rturn_released();
    }
    else if(e->key() == Qt::Key_4){
        if(e->isAutoRepeat()) return;
        on_lmove_released();
    }
    else if(e->key() == Qt::Key_6){
        if(e->isAutoRepeat()) return;
        on_rmove_released();
    }
    else if(e->key() == Qt::Key_5){
        if(e->isAutoRepeat()) return;
        on_down_released();
    }
    else if(e->key() == Qt::Key_0){
        if(e->isAutoRepeat()) return;
        on_up_released();
    }
}


void ControlView::Update(int id)
{
    switch (id) {
    case ORD1_XSENS:
        ui->roll_info->setValue(g::roll);
        ui->pitch_info->setValue(g::pitch);
        ui->yaw_info->setValue(g::yaw);
        break;
    case ORD1_KELLER:
        ui->z_info->setValue(g::deep/1000.0); //need product a factor
        break;
    case ORD1_ALT:

        break;
    default:
        break;
    }
}

float ControlView::P2N(float p)
{
    //PWM % to propeller speed r/min
    return 18.48*p;
}

float ControlView::N2T(float n, short dir)
{
    //propeller speed r/min to thrust N
    float T = 1.926e-5*n*abs(n);
    return dir*T;
}

float ControlView::P2T(float p, short dir)
{
    //PWM %to thrust N
    return N2T(P2N(p),dir);
}

float ControlView::N2P(float n)
{
    return n/18.48;
}

float ControlView::T2N(float t, short dir)
{
    return t*dir >= 0? sqrt(t*dir/1.926e-5):-sqrt(-t*dir/1.926e-5);
}

float ControlView::T2P(float t, short dir)
{
    return N2P(T2N(t,dir));
}

Vector8f ControlView::ThrustAllocation(Vector6f input)
{
    g::J = GetJ(g::roll,g::pitch);
    return g::K_star*g::J.inverse()*input;
}

void ControlView::SendMotorSpeed()
{
    char sendbuf[38];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE;
    sendbuf[2] = 34;
    sendbuf[3] = ORD1_MOTOR;
    sendbuf[4] = MOTOR_ORDER;
    for(int i = 0; i < 8; i++){
        if(abs(g::motorvec[i].pwm) > 95)
            g::motorvec[i].pwm = g::motorvec[i].pwm >= 0? 95 : 95;
        memcpy(&sendbuf[5 + 4*i],&(g::motorvec[i].pwm),4);
    }
    sendbuf[37] = 0x00;

    if(g::isConnected == true)
        g::tcpSocket->write(sendbuf,38);
}

Matrix66f ControlView::GetJ(float roll, float pitch)
{
    float phi = roll*PI/180.0; float theta = pitch*PI/180.0; float psi = 0;
    Matrix66f J;
    J << cos(psi)*cos(theta) , -sin(psi)*cos(phi)+cos(psi)*sin(theta)*sin(phi) ,  sin(psi)*sin(phi)+cos(psi)*cos(phi)*sin(theta)   ,       0 ,                0             ,                      0   ,
         sin(psi)*cos(theta) , cos(psi)*cos(phi)+sin(phi)*sin(theta)*sin(psi)  , -cos(psi)*sin(phi)+sin(theta)*sin(psi)*cos(phi)   ,       0 ,                0             ,                      0   ,
            -sin(theta)      ,                cos(theta)*sin(phi)              ,                          cos(theta)*cos(phi)      ,       0 ,                0             ,                      0   ,
                  0          ,                               0                 ,                               0                   ,       1 ,         sin(phi)*tan(theta)  ,       cos(phi)*tan(theta),
                  0          ,                               0                 ,                               0                   ,       0 ,            cos(phi)          ,               -sin(phi)  ,
                  0          ,                               0                 ,                               0                   ,       0 ,         sin(phi)/cos(theta)  ,       cos(phi)/cos(theta);

    return J;
}



void ControlView::on_forward_pressed()
{
//    static int i = 0;
//    std::cout << i++ <<" :hello" <<std::endl;
    g::Fx = 150;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
        std::cout << i<<": " << g::motorvec[i].pwm <<" "<< f(i) << "==";
    }
    std::cout <<std::endl;
    SendMotorSpeed();
}

void ControlView::on_forward_released()
{
    g::Fx = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_backward_pressed()
{
//    static int i = 0;
//    std::cout << i++ <<" :hello" <<std::endl;
    g::Fx = -150;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}



void ControlView::on_backward_released()
{
    g::Fx = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_lturn_pressed()
{
    g::Tz = -80;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_lturn_released()
{
    g::Tz = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_rturn_pressed()
{
    g::Tz = 80;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_rturn_released()
{
    g::Tz = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_lmove_pressed()
{
    g::Fy = -105;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_lmove_released()
{
    g::Fy = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}


void ControlView::on_rmove_pressed()
{
    g::Fy = 105;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_rmove_released()
{
    g::Fy = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_orien_ctrl_btn_clicked()
{
    if(!bOrienCtrl){
        std::cout << "ORIENTATION CONTROL BEGAN!!" << std::endl;

        RollPID.setRefreshInterval(200);
        RollPID.setKp(ui->rp->value());
        RollPID.setKi(ui->ri->value());
        RollPID.setKd(ui->rd->value());
        RollPID.setErrorThreshold(2.0);
        RollPID.setOutputLowerLimit(-TXMAX/3.0);
        RollPID.setOutputUpperLimit(TXMAX/3.0);
        RollPID.setDeadZone(10);

        PitchPID.setRefreshInterval(200);
        PitchPID.setKp(ui->pp->value());
        PitchPID.setKi(ui->pi->value());
        PitchPID.setKd(ui->pd->value());
        PitchPID.setErrorThreshold(2.0);
        PitchPID.setOutputLowerLimit(-TYMAX/6.0);
        PitchPID.setOutputUpperLimit(TYMAX/6.0);
        PitchPID.setDeadZone(10);

        ot = new QTimer(this);
        connect(ot,SIGNAL(timeout()),this,SLOT(OrientationControl()));
        ot->start(250);
        ui->orien_ctrl_btn->setText("Stop");
        bOrienCtrl = true;
    }
    else{
        ot->stop();
        ot->destroyed();
        for(int i = 0; i < 8; i++){
            g::motorvec[i].pwm = 0;
        }
        SendMotorSpeed();
        g::Tx = 0; g::Ty = 0;
        ui->orien_ctrl_btn->setText("Start");
        bOrienCtrl = false;

    }
}


void ControlView::on_down_pressed()
{
    g::Fz = 120;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
        std::cout << i<<": " << g::motorvec[i].pwm <<" "<< f(i) << "==";
    }
    std::cout <<std::endl;
    SendMotorSpeed();
}

void ControlView::on_down_released()
{
    g::Fz = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_up_pressed()
{
    g::Fz = -200;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::on_up_released()
{
    g::Fz = 0;
    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::OrientationControl()
{
    float rdp = ui->roll_ctrl->value();
    float pdp = ui->pitch_ctrl->value();
    RollPID.setDesiredPoint(ui->roll_ctrl->value());
    PitchPID.setDesiredPoint(ui->pitch_ctrl->value());

    float fytemp = 0;

    //if(abs(rdp - g::roll) > 45){
    if(rdp >= 70){
        RollPID.setKp(0.5);
        RollPID.setKi(0.05);
        RollPID.setKd(0.1);
        RollPID.setOutputLowerLimit(-TXMAX);
        RollPID.setOutputUpperLimit(TXMAX);

        if(g::roll >= 60)
            fytemp = -170;
        else
            fytemp = 0;
    }
    else{
        RollPID.setKp(ui->rp->value());
        RollPID.setKi(ui->ri->value());
        RollPID.setKd(ui->rd->value());
        RollPID.setOutputLowerLimit(-TXMAX/3.0);
        RollPID.setOutputUpperLimit(TXMAX/3.0);
    }

    g::Tx = RollPID.refresh(g::roll);
    g::Ty = PitchPID.refresh(g::pitch);


    Vector6f T;
    T << g::Fx, g::Fy+fytemp, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
       // g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir) > 99.99 ? 99.99:T2P(f(i),g::motorvec[i].dir);
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}

void ControlView::PositionControl()
{
    ZPID.setDesiredPoint(ui->z_ctrl->value()*1000);

    g::Fz = ZPID.refresh(g::deep);

    Vector6f T;
    T << g::Fx, g::Fy, g::Fz, g::Tx, g::Ty, g::Tz;
    Vector8f f = ThrustAllocation(T);
    for(int i = 0; i < 8; i++){
       // g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir) > 99.99 ? 99.99:T2P(f(i),g::motorvec[i].dir);
        g::motorvec[i].pwm = T2P(f(i),g::motorvec[i].dir);
    }
    SendMotorSpeed();
}



void ControlView::on_pos_ctrl_btn_clicked()
{
    if(!bPosCtrl){
        std::cout << "DEEP CONTROL BEGAN!!" << std::endl;

        ZPID.setRefreshInterval(200);
        ZPID.setKp(ui->zzp->value());
        ZPID.setKi(ui->zzi->value());
        ZPID.setKd(ui->zzd->value());
        ZPID.setErrorThreshold(100.0);
        ZPID.setOutputLowerLimit(-FZMAX/1.5);
        ZPID.setOutputUpperLimit(FZMAX/1.5);
        ZPID.setDeadZone(10);

        pt = new QTimer(this);
        connect(pt,SIGNAL(timeout()),this,SLOT(PositionControl()));
        pt->start(200);
        ui->pos_ctrl_btn->setText("Stop");
        bPosCtrl = true;
    }
    else{
        pt->stop();
        pt->destroyed();
        for(int i = 0; i < 8; i++){
            g::motorvec[i].pwm = 0;
        }
        SendMotorSpeed();
        g::Fz = 0;
        ui->pos_ctrl_btn->setText("Start");
        bPosCtrl = false;

    }
}

void ControlView::on_turnleft_clicked()
{
    ui->roll_ctrl->setValue(90.0);
    if(!bOrienCtrl){
        on_orien_ctrl_btn_clicked();
    }

    if(bPosCtrl){
      //  on_pos_ctrl_btn_clicked();
    }
}

void ControlView::on_turnright_clicked()
{
    ui->roll_ctrl->setValue(-90.0);
    if(!bOrienCtrl){
        on_orien_ctrl_btn_clicked();
    }

    if(bPosCtrl){
     //   on_pos_ctrl_btn_clicked();
    }
}
