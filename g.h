#ifndef G_H
#define G_H

#include <vector>
#include <utility>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <QtNetwork>
#include <Dense>

#define CAMERA1_IP   "192.168.0.12"
#define CAMERA2_IP   "192.168.0.13"

#define SERVER_PORT   6666
#define SERVER_IP     "192.168.0.100"
#define LOCAL_IP      "192.168.0.1"
#define TCP_SEND_LEN 50
#define TCP_RECIEVE_LEN 50

#define XSENS_STEP    0.1
#define KELLER_STEP   0.1
#define CONTROL_STEP  0.2


#define ORD1_MOTOR             0x01
#define ORD1_PLATFORM          0x02
#define ORD1_XSENS             0x03
#define ORD1_KELLER            0x04
#define ORD1_ALT               0x05
#define ORD1_TEMP              0x06
#define ORD1_LIGHT             0x07

#define MOTOR_INFO             0x00
#define MOTOR_ORDER            0x01

#define PLATFORM_INFO          0x00
#define PLATFORM_POS_ORDER     0x01
#define PLATFORM_SET_XYSPEED   0x02
#define PLATFORM_SET_ZSPEED    0x03
#define PLATFORM_SET_WSPEED    0x04
#define PLATFORM_SET_XZERO     0x05
#define PLATFORM_SET_YZERO     0x06
#define PLATFORM_SET_ZZERO     0X07

#define XSENS_DATA             0X00

#define KELLER_DATA            0X00

#define ALT_DATA               0X00

#define TEMP_DATA              0X00
#define TEMP_ORDER             0x01

#define LIGHT_DATA1            0x00
#define LIGHT_SET1             0x01
#define LIGHT_DATA2            0x02
#define LIGHT_SET2             0x03


typedef Eigen::Matrix<float,6,8> Matrix68f;
typedef Eigen::Matrix<float,8,6> Matrix86f;
typedef Eigen::Matrix<float,6,6> Matrix66f;
typedef Eigen::Matrix<float,6,1> Vector6f;
typedef Eigen::Matrix<float,8,1> Vector8f;
typedef Eigen::Vector3f          Vector3f;
typedef Eigen::Matrix<float,6,1> Vector6f;


struct Motor
{
    short int   status; // motor status : fault==>-1, stop==>0, start==>2, enable==>1
    float       pwm; //-99.99~99.99 %
    short int   speed; //-9999~9999 r/min
    short int   current; //0 ~ 9999mA
    short int   dir;
};

struct InterStr
{
  float x;
  float y;
};

struct RECXsensData
{
  float roll;
  float pitch;
  float yaw;
};

struct RECAltData
{
  float alt1_distance;
  float alt1_energy;
  float alt1_correlation;
  float alt2_distance;
  float alt2_energy;
  float alt2_correlation;
};

struct RECCtrlData
{
  float fx;
  float fy;
  float fz;
  float tx;
  float ty;
  float tz;
};

struct Pos
{
  float x;
  float y;
  float z;
};


class g
{
public:
    g();

    /******TCP******/
    static QTcpSocket* tcpSocket;
    static int connCnt;
    static bool isConnected;

    /******xsens******/
    static float roll;
    static float pitch;
    static float yaw;

    static float roll_pre;
    static float pitch_pre;
    static float yaw_pre;

    static float droll;
    static float dpitch;
    static float dyaw;

    static float droll_pre;
    static float dpitch_pre;
    static float dyaw_pre;

    /******can******/
    static std::vector<Motor> motorvec;
    static float cabin_temp;
    static float wall_temp;

    /******platform******/
    // Axises Target Position
    static float TargetPosition1;
    static float TargetPosition2;
    static float TargetPosition3;
    // Axises Speed
    static int AxisSpeedXY;
    static int AxisSpeedZ;
    static int WireFeedSpeed;
    //Axises Current Location
    static float CurrentPosition1;
    static float CurrentPosition2;
    static float CurrentPosition3;
    //Interpolation
    static std::vector<InterStr> intervec;
    static int it;

    /******keller******/
    static float pressval; // pressure value
    static float tempval;  // tempreture value
    static float deep;

    static float deep_pre;
    static float ddeep;
    static float ddeep_pre;

    /******light******/
    static short int brightness[2]; //current brightness value from 0 to 76==> 0x00 ~ 0x4c
    static short int lighttemp[2];

    /******altimeter******/
    static float distance[2];
    static float energy[2];
    static float correlation[2];
    static float temperature[2];
    static float watertemp;

    /******control*******/
    static Matrix68f K;
    static Matrix86f K_star;
    static Matrix66f J;
    static float  Fx;
    static float  Fy;
    static float  Fz;
    static float  Tx;
    static float  Ty;
    static float  Tz;

    static Matrix66f M;
    static Matrix66f C;
    static Matrix66f G;

    /******position*******/
    static float x;
    static float y;
    static float z;

};

#endif // G_H
