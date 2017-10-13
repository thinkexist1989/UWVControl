#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <QThread>


class interpolation: public QThread
{
public:
    interpolation();
    void run();

    static void LinearInterpolation(float x0, float y0, float xe, float ye);
    static void LinearInterpolation2(float x0, float y0, float xe, float ye);
    static void CircularInterpolation(float x0, float y0, float xe, float ye, float x, float y, int shun);

private:
    static void StepMoving();

};

#endif // INTERPOLATION_H
