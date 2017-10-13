#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#define PI 3.1415926
#define POOL_LENGTH 8
#define POOL_WIDTH  4

#include "Eigen/Dense"

typedef Eigen::Matrix<float,3,4> Matrix34f;
typedef Eigen::Vector3f          Vector3f;
typedef Eigen::Matrix3f          Matrix3f;

class Localization
{
    public:
        Localization();
       ~Localization();

       Vector3f Calc_xy(float roll, float pitch, float yaw, float front_dis, float side_dis);
       void SetBias(float biasangle);
       void SetPhi(float roll);
       void SetTheta(float pitch);
       void SetPsi(float yaw);

    private:
        float beta; // rotation angle
        
        float phi; //roll angle in radian
        float theta; //pitch angle in radian
        float psi; //yaw angle in radian

        float phiprev;
        float thetaprev;
        float psiprev;

        float xprev;
        float yprev;

        float front_dis;
        float side_dis;

        float l;
        float w;

        float bias; //set bias value

        Matrix3f GetJ(Vector3f orivec);
        Vector3f GetVec(Vector3f orivec, Vector3f altvec);
        Vector3f front_vector(Vector3f orivec);
        Vector3f side_vector(Vector3f orivec);
        Matrix3f GetRotationMatrix(float beta);
};


#endif // LOCALIZATION_H
