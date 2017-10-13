#include "interpolation.h"
#include "g.h"
#include <iostream>


interpolation::interpolation()
{
}

void interpolation::run()
{
    LinearInterpolation(20, 20, 30, 30);
}

void interpolation::LinearInterpolation(float x0, float y0, float xe, float ye)
{
    float Nxy,xi=0,yi=0,FM,F;
    int i=1;
    float steplen = 1.5;
   // Nxy = sqrt((xe-x0)^2+(ye-y0)^2)/steplen;
    Nxy =abs( (xe - x0)/steplen) + abs((ye - y0)/steplen);
    xi = x0;//x,y分别存放实时的坐标
    yi = y0;//
    //FM =xe*yi-xi*ye;
    InterStr t;
    t.x = x0;
    t.y = y0;
    g::intervec.push_back(t);

    while(Nxy!=0)
    {
        FM =(xe-x0)*(yi-y0)-(xi-x0)*(ye-y0);
        if(xe-x0==0){}
        else if((ye-y0)/(xe-x0)<0)//调整斜率
                FM=FM*(-1);
        if(FM >= 0 )
            {
            //F=FM-ye;
                if(xe-x0==0){
                        if(ye-y0>0)
                            yi=yi+steplen; //Y正向走步程序
                        else if(ye-y0<0)
                            yi=yi-steplen;//Y负向走步程序
                        //cout<<i<<"  "<<xi<<","<<yi<<"  \tFM="<<FM<<"  "<<endl;
                }

                else if(xe-x0>0)
                xi=xi+steplen; //X正向走步程序
                else if(xe-x0<0)
                    xi=xi-steplen;//X负向走步程序
//                std::cout<<i<<":  "<<xi<<","<<yi<<"  \tFM="<<FM<<"  "<<std::endl;
            }
        else
            {
                //F=FM+xe;
                if(ye-y0>0)
                yi=yi+steplen; //Y正向走步程序
                else if(ye-y0<0)
                    yi=yi-steplen;//Y负向走步程序
//                std::cout<<i<<":  "<<xi<<","<<yi<<"  \tFM="<<FM<<"  "<<std::endl;
            }
        std::cout<<i<<":  "<<xi<<","<<yi<<"  \tFM="<<FM<<"  "<<std::endl;
        if(i%2 == 0){
            InterStr t;
            t.x = xi;
            t.y = yi;
            g::intervec.push_back(t);
        }

        Nxy--;
        i++;
        }
    t.x = xe;
    t.y = ye;
    g::intervec.push_back(t);
}

void interpolation::LinearInterpolation2(float x0, float y0, float xe, float ye)
{
    float Nxy,xi=0,yi=0,FM,F;
    int i=1;
    float steplen = 0.75;
    Nxy = sqrt(pow((xe-x0),2)+pow((ye-y0),2))/steplen;
    //Nxy =abs( (xe - x0)/steplen) + abs((ye - y0)/steplen);
    xi = x0;//x,y分别存放实时的坐标
    yi = y0;//
    //FM =xe*yi-xi*ye;
//    InterStr t;
//    t.x = x0;
//    t.y = y0;
//    g::intervec.push_back(t);

    for(int i = 0; i < (int)Nxy ; i++){
        xi = x0 + (xe - x0)/Nxy*i;
        yi = y0 + (ye - y0)/Nxy*i;
        InterStr t; t.x = xi; t.y = yi;
        g::intervec.push_back(t);
    }
    InterStr t;
    t.x = xe;
    t.y = ye;
    g::intervec.push_back(t);
}

void interpolation::CircularInterpolation(float x0, float y0, float xe, float ye, float x, float y, int shun)
{
    float xi=0,yi=0,Fe,rr=NULL;//rr为半径的平方
    int i = 1;
    float steplen = 0.5;
    if ((xe - x)*(xe - x) + (ye - y)*(ye - y) == (x0 - x)*(x0 - x) + (y0 - y)*(y0 - y))
        rr = (xe - x)*(xe - x) + (ye - y)*(ye - y);
    else{
        std::cout << "您输入的坐标够不成圆弧！" << std::endl << "请重新输入" << std::endl;
        return;
    }
    xi=x0;yi=y0;//起点赋给xiyi
    while(xi!=xe||yi!=ye)//没有到达终点时
    {
        Fe=(xi-x)*(xi-x)+(yi-y)*(yi-y)-rr;
        //cout<<"Fe:"<<Fe<<endl;//system("pause");
        if(Fe>=0)
        {
            if(shun==1){
                if(xi>=x&&yi<=y)
                    xi-=steplen;
                else if(xi<=x&&yi<=y)
                    yi+=steplen;
                else if(xi<=x&&yi>=y)
                    xi+=steplen;
                else if(xi>=x&&yi>=y)
                    yi-=steplen;
            }
            else if(shun==2){
                if(xi>=x&&yi<=y)
                    yi+=steplen;//xi--;
                else if(xi<=x&&yi<=y)
                    xi+=steplen;//yi++;
                else if(xi<=x&&yi>=y)
                    yi-=steplen;//xi++;
                else if(xi>=x&&yi>=y)
                    xi-=steplen;//yi--;
            }
        }
        else
        {
            if(shun==1){
                if(xi>=x&&yi<=y)
                    yi-=steplen;//xi--;
                else if(xi<=x&&yi<=y)
                    xi-=steplen;//yi++;
                else if(xi<=x&&yi>=y)
                    yi+=steplen;//xi++;
                else if(xi>=x&&yi>=y)
                    xi+=steplen;//yi--;
            }
            else if(shun==2){
                if(xi>=x&&yi<=y)
                    xi+=steplen;//yi++;//xi--;
                else if(xi<=x&&yi<=y)
                    yi-=steplen;//xi++;//yi++;
                else if(xi<=x&&yi>=y)
                    xi-=steplen;//yi--;//xi++;
                else if(xi>=x&&yi>=y)
                    yi+=steplen;//xi--;//yi--;
            }

        }
//        std::cout<<i<<":\t"<<xi<<","<<yi<<std::endl;
        if(i%2 == 0){
            InterStr t;
            t.x = xi;
            t.y = yi;
            g::intervec.push_back(t);
            std::cout<<i<<":\t"<<xi<<","<<yi<<std::endl;
        }
        i++;
    }
    //cout<<x0<<y0<<xe<<ye<<xi<<yi<<x<<y<<Fe<<i<<shun<<rr;
}

//void interpolation::LinearInterpolation(float x0, float y0, float xe, float ye)
//{
//    float Nxy,xi=0,yi=0,FM;                     //F
//    int i=1;

//    Nxy =abs( (xe - x0)) + abs((ye - y0));      //Total times of step moving
//    g::TargetPosition1 = x0;                    //Move Platform to the Start Point.
//    g::TargetPosition2 = y0;                    //(Be careful of the Z axis position)
//    StepMoving();
//    //FM =xe*yi-xi*ye;

//    while(Nxy>1)
//    {
//        FM =(xe-x0)*(g::TargetPosition2-y0)-(g::TargetPosition1-x0)*(ye-y0);
//        if(xe-x0==0){}
//        else if((ye-y0)/(xe-x0)<0)                                      //Adjustment slope
//                FM=FM*(-1);
//        if(FM >= 0 ){
//            //F=FM-ye;
//            if(xe-x0==0){
//                    if(ye-y0>0)
//                        g::TargetPosition2 = g::TargetPosition2+1;      //Y forward moving
//                    else if(ye-y0<0)
//                        g::TargetPosition2 = g::TargetPosition2-1;      //Y backward moving
//            }
//            else if(xe-x0>0)
//                g::TargetPosition1 = g::TargetPosition1+1;              //X forward moving
//            else if(xe-x0<0)
//                g::TargetPosition1 = g::TargetPosition1-1;              //X backward moving
//            std::cout<<i<<":  "<<g::TargetPosition1<<","<<g::TargetPosition2<<"  \tFM="<<FM<<"  "<<std::endl;
//            StepMoving();
//        }
//        else{
//            //F=FM+xe;
//            if(ye-y0>0)
//            g::TargetPosition2 = g::TargetPosition2+1;                  //Y forward moving
//            else if(ye-y0<0)
//                g::TargetPosition2 = g::TargetPosition2-1;              //Y backward moving
//            std::cout<<i<<":  "<<g::TargetPosition1<<","<<g::TargetPosition2<<"  \tFM="<<FM<<"  "<<std::endl;
//            StepMoving();
//        }
//        Nxy--;
//        i++;
//    }
//}


//void interpolation::CircularInterpolation(float x0, float y0, float xe, float ye, float x, float y, int shun)
//{
//    float Fe=0, rr=0;//rr为半径的平方
//    int  i = 1;
//    if ((xe - x)*(xe - x) + (ye - y)*(ye - y) == (x0 - x)*(x0 - x) + (y0 - y)*(y0 - y))
//        rr = (xe - x)*(xe - x) + (ye - y)*(ye - y);
//    else{
//        std::cout << "您输入的坐标够不成圆弧！" << std::endl << "请重新输入" << std::endl;
//        return;
//    }
//    g::TargetPosition1 = x0;                    //Move Platform to the Start Point.
//    g::TargetPosition2 = y0;                    //(Be careful of the Z axis position)
//    StepMoving();

//    while ((g::CurrentPosition1-xe)>1.0 || (g::CurrentPosition2-ye)>1.0)//没有到达终点时
//    {
//        Fe = (g::CurrentPosition1 - x)*(g::CurrentPosition1 - x) + (g::CurrentPosition2 - y)*(g::CurrentPosition2 - y) - rr;
//        if (Fe >= 0)
//        {
//            if (shun == 1) {
//                if (g::CurrentPosition1 >= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition1 = g::TargetPosition1-1;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition2 = g::TargetPosition2+1;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition1 = g::TargetPosition1+1;
//                else if (g::CurrentPosition1 >= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition2 = g::TargetPosition2-1;
//            }
//            else if (shun == 2) {
//                if (g::CurrentPosition1 >= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition2 = g::TargetPosition2+1;//xi--;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition1 = g::TargetPosition1+1;//yi++;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition2 = g::TargetPosition2-1;//xi++;
//                else if (g::CurrentPosition1 >= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition1 = g::TargetPosition1-1;//yi--;
//            }
//        }
//        else
//        {
//            if (shun == 1) {
//                if (g::CurrentPosition1 >= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition2 = g::TargetPosition2-1;//xi--;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition1 = g::TargetPosition1-1;//yi++;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition2 = g::TargetPosition2+1;//xi++;
//                else if (g::CurrentPosition1 >= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition1 = g::TargetPosition1+1;//yi--;
//            }
//            else if (shun == 2) {
//                if (g::CurrentPosition1 >= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition1 = g::TargetPosition1+1;//yi++;//xi--;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 <= y)
//                    g::TargetPosition2 = g::TargetPosition2-1;//xi++;//yi++;
//                else if (g::CurrentPosition1 <= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition1 = g::TargetPosition1-1;//yi--;//xi++;
//                else if (g::CurrentPosition1 >= x&&g::CurrentPosition2 >= y)
//                    g::TargetPosition2 = g::TargetPosition2+1;//xi--;//yi--;
//            }
//        }
//        std::cout << i << ":\t" << g::TargetPosition1 << "," << g::TargetPosition2 << std::endl;
//        StepMoving();
//        i++;
//    }
//}


/***********StepMoving**************/
void interpolation::StepMoving()
{
    char sendbuf[18];
    sendbuf[0] = 0xFF; sendbuf[1] = 0xFE; //Preamble
    sendbuf[2] = 14; //LEN = ord1+ord2+data
    sendbuf[3] = ORD1_PLATFORM; sendbuf[4] = PLATFORM_POS_ORDER;
    memcpy(&sendbuf[5],&g::TargetPosition1,4);
    memcpy(&sendbuf[9],&g::TargetPosition2,4);
    memcpy(&sendbuf[13],&g::TargetPosition3,4);
    sendbuf[17] = 0x00; //CRC
    if(g::isConnected){
        int n = g::tcpSocket->write(sendbuf, 18);
        if(n == 18){
            std::cout<<"PLATFORM:Order send succeed"<<std::endl;
        }
    }
    else{
        std::cout<<"PLATFORM:Order send faile"<<std::endl;
    }

}
