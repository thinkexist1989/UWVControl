#include "qpaintlabel.h"
#include <QTimer>
#include <QPainter>
#include <g.h>

QPaintLabel::QPaintLabel(QWidget *parent):QLabel(parent)
{
    QTimer* t = new QTimer(this);
    connect(t,SIGNAL(timeout()),this,SLOT(update()));
    t->start(50);
}

void QPaintLabel::paintEvent(QPaintEvent *event)
{
    PaintUWV(x,y,angle);
    angle += 2;
    x = 1.1*cos((angle-90)/180.0*PI);
    y = 1.1*sin((angle-90)/180.0*PI);
   // x += 0.005;
}


void QPaintLabel::PaintUWV(float x, float y, float angle)
{

    int width = this->width();
    int height = this->height();
    QPixmap pix(":uwv");
    QPainter painter(this);
    painter.save();

    float cx = (x + POOL_LENGTH/2.0)*width/POOL_LENGTH;
    float cy = (y + POOL_WIDTH/2.0)*height/POOL_WIDTH;
    painter.translate(cx,cy);
    painter.rotate(angle);
    painter.drawPixmap(-0.5*width/POOL_LENGTH,-0.5*height/POOL_WIDTH,width/POOL_LENGTH,height/POOL_WIDTH,pix);
    painter.restore();



//    QRect rotatedRect(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());

//    int cx = rect.x() + rect.width() / 2;
//    int cy = rect.y() + rect.height() / 2;

//    painter->save();
//    painter.setBrush(Qt::yellow);

//    painter->translate(cx, cy);
//    painter->rotate(angle);
//    painter->drawRect(rotatedRect);
//    painter->restore();

}
