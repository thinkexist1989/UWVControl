#ifndef QPAINTLABEL_H
#define QPAINTLABEL_H

#include <QLabel>
#include <localization.h>

class QPaintLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QPaintLabel(QWidget *parent = 0);

    float angle;
    float x;
    float y;


    void PaintUWV(float x, float y, float angle);

signals:

public slots:


protected:
    void paintEvent(QPaintEvent *event);

};

#endif // QPAINTLABEL_H
