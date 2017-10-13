#include "localization.h"

Localization::Localization():
    beta(0),phi(0),theta(0),psi(0),phiprev(0),thetaprev(0),psiprev(0),xprev(0),yprev(0),l(POOL_LENGTH/2.0),w(POOL_WIDTH/2.0),bias(0)
{    
}

Localization::~Localization()
{
}

Vector3f Localization::Calc_xy(float roll, float pitch, float yaw, float front_dis, float side_dis)
{
    SetPhi(roll); SetTheta(pitch); SetPsi(yaw);
    Vector3f orivec(phi,theta,psi);
    Vector3f fv = front_vector(orivec);
    Vector3f sv = side_vector(orivec);

    Matrix34f n; // normal vectors
    n << 1 , 0 , -1 ,  0, 
         0 , 1 ,  0 , -1,
         0 , 0 ,  0 ,  0;
    Matrix34f p; //
    p << l , 0 , -l ,  0,
         0 , w ,  0 ,  w,
         0 , 0 ,  0 ,  0;
    Matrix34f c; //corners
    c << l ,-l , -l ,  l,
         w , w , -w , -w,
         0 , 0 ,  0 ,  0;

    Vector3f ns(0,0,1);
    int R = 0, B = 1, L = 2, U = 3;
    if((psi >= 0)&&(psi < PI/2)){
        beta = 0; R = 0; B = 1; L = 2; U = 3;
    }
    else if((psi >= PI/2)&&(psi < PI)){
        beta = PI/2; R = 1; B = 2; L = 3; U = 0;
    }
    else if((psi >= -PI/2)&&(psi < 0)){
        beta = -PI/2; R = 3; B = 0; L = 1; U = 2;
    }
    else if((psi >= -PI)&&(psi < PI/2)){
        beta = -PI; R = 2; B = 3; L = 0; U = 1;
    }

    Matrix3f C = GetRotationMatrix(beta);
    float alpha = psi - beta;
    float alphaprev = psiprev - beta;
    Vector3f locprev(xprev,yprev,0);
    Vector3f xlocprev = C*locprev;
    Matrix34f nn = C*n;
    Matrix34f pp = C*p;
    Matrix34f cc = C*c;
    
    float xxprev = xlocprev(0);
    float yyprev = xlocprev(1);

    float ddff = front_dis*(1-pow(abs(fv.dot(ns))/(fv.norm()*ns.norm()),2.0));
    float ddss = side_dis*(1-pow(abs(sv.dot(ns))/(sv.norm()*ns.norm()),2.0));

    float k1 = (yyprev - cc(1,R))/(xxprev - cc(0,R));
    float k2 = (yyprev - cc(1,B))/(xxprev - cc(0,B));

    int region = 0;
    if((k1 >= tan(alpha))&&(k2 >= -1/tan(alpha)))
        region = 1;
    else if((k1 >= tan(alpha))&&(k2 < -1/tan(alpha)))
        region = 2;
    else if((k1 < tan(alpha))&&(k2 < -1/tan(alpha)))
        region = 3;
    else if((k1 < tan(alpha))&&(k2 >= -1/tan(alpha)))
        region = 4;

    float dr = abs(cos(alpha)*(yyprev-cc(1,R))-sin(alpha)*(xxprev-cc(0,R)));
    float db = abs(sin(alpha)*(yyprev-cc(1,B))+cos(alpha)*(xxprev-cc(0,B)));
    float tr = 0, tb = 0;
    if(dr< 0.3)
        tr = 0.8;   
    if(db< 0.3)
        tb = 0.8;

    float xx,yy;
    switch(region){
        case 1:
            xx = pp.col(R).norm() - ddff*cos(alpha);
            yy = pp.col(B).norm() - ddss*cos(alpha);
            break;
        case 2:
            xx = pp.col(R).norm() - ddff*cos(alpha);
            yy = yyprev + (xx - xxprev)*tan((alphaprev + alpha)/2);
            break;
        case 3:
            xx = -pp.col(L).norm() + ddss*sin(alpha);
            yy = pp.col(B).norm() - ddff*sin(alpha);
            break;
        case 4:
            yy = pp.col(B).norm() - ddff*sin(alpha);
            xx = xxprev + (yy - yyprev)/tan((alphaprev+alpha)/2);
            break;
        default:
            break;
    }
    Vector3f xloc(xx,yy,0);
    Vector3f loc = C.inverse()*xloc;
    xprev = loc(0); yprev = loc(1);
    phiprev = phi; thetaprev = theta; psiprev = psi;

    return loc;



}


Matrix3f Localization::GetJ(Vector3f orivec)
{
    float phi = orivec(0); float theta = orivec(1); float psi = orivec(2);
    Matrix3f J;
    J << cos(psi)*cos(theta) , -sin(psi)*cos(phi)+cos(psi)*sin(theta)*sin(phi) ,  sin(psi)*sin(phi)+cos(psi)*cos(phi)*sin(theta)   ,
         sin(psi)*cos(theta) , cos(psi)*cos(phi)+sin(phi)*sin(theta)*sin(psi)  , -cos(psi)*sin(phi)+sin(theta)*sin(psi)*cos(phi)   ,
            -sin(theta)      ,                cos(theta)*sin(phi)              ,                          cos(theta)*cos(phi)      ;
    
    return J;
}

Vector3f Localization::GetVec(Vector3f orivec, Vector3f altvec)
{
    return GetJ(orivec)*altvec;
}
    
Vector3f Localization::front_vector(Vector3f orivec)
{
    Vector3f altvec(1,0,0);
    return GetVec(orivec,altvec);
}
    
Vector3f Localization::side_vector(Vector3f orivec)
{
    Vector3f altvec(0,1,0);
    return GetVec(orivec,altvec);
}

void Localization::SetBias(float biasangle)
{
    bias = biasangle/180.0*PI;
}

void Localization::SetPhi(float roll)
{
    phi = roll/180.0*PI;
}

void Localization::SetTheta(float pitch)
{
    theta = pitch/180.0*PI;
}

void Localization::SetPsi(float yaw)
{
    float tmp = (yaw-bias)/180.0*PI;
    if(tmp >= PI)
        psi = tmp - 2*PI;
    else if(tmp < -PI)
        psi = tmp + 2*PI;
}

Matrix3f Localization::GetRotationMatrix(float beta)
{
    Matrix3f C;
    C << cos(beta)  ,sin(beta) ,  0,
         -sin(beta) ,cos(beta) ,  0,
            0       ,    0     ,  1;
    return C;
}
