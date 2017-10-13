#include "g.h"
#include <QMutex>


QMutex tcpmutex;

g::g(){
}

/***    tcpip    ***/
QTcpSocket* g::tcpSocket = new QTcpSocket;
int g::connCnt = 0;
bool g::isConnected = false;

/***    xsens    ***/
float g::roll = 0;
float g::pitch = 0;
float g::yaw = 0;

/***    can    ***/
std::vector<Motor> g::motorvec(8);
float g::cabin_temp;
float g::wall_temp;

/******platform******/
// Axises Target Position
float g::TargetPosition1 = 0;
float g::TargetPosition2 = 0;
float g::TargetPosition3 = 0;
// Axises Speed
int g::AxisSpeedXY = 500;
int g::AxisSpeedZ = 300;
int g::WireFeedSpeed = 300;
//Axises Current Location
float g::CurrentPosition1 = 0;
float g::CurrentPosition2 = 0;
float g::CurrentPosition3 = 0;
//Interpolation
std::vector<InterStr> g::intervec;
int g::it = 0;

/******keller******/
float g::pressval; // pressure value
float g::tempval;  // tempreture value
float g::deep;

/******light******/
short int g::brightness[2]; //current brightness value from 0 to 76==> 0x00 ~ 0x4c
short int g::lighttemp[2];

/******altimeter******/
float g::distance[2];
float g::energy[2];
float g::correlation[2];
float g::temperature[2];
float g::watertemp;


/******control*******/
Matrix68f g::K;
Matrix86f g::K_star;
Matrix66f g::J;
float g::Fx = 0;
float g::Fy = 0;
float g::Fz = 0;
float g::Tx = 0;
float g::Ty = 0;
float g::Tz = 0;

/******position*******/
float g::x = 0;
float g::y = 0;
float g::z = 0;

