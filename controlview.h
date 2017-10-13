#ifndef CONTROLVIEW_H
#define CONTROLVIEW_H

#include <QWidget>
#include "g.h"
#include <QTimer>
#include <PID.h>

namespace Ui {
class ControlView;
}

class ControlView : public QWidget
{
    Q_OBJECT

public:
    explicit ControlView(QWidget *parent = 0);
    ~ControlView();

    float P2N(float p);
    float N2T(float n, short dir);
    float P2T(float p,short dir);

    float N2P(float n);
    float T2N(float t, short dir);
    float T2P(float t, short dir);

    Vector8f ThrustAllocation(Vector6f input);
    void SendMotorSpeed();

    greg::PID RollPID;
    greg::PID PitchPID;
    greg::PID YawPID;
    greg::PID XPID;
    greg::PID YPID;
    greg::PID ZPID;

    bool bOrienCtrl;
    bool bPosCtrl;

    QTimer *ot;
    QTimer *pt;

    Matrix66f GetJ(float roll, float pitch);

private:
    Ui::ControlView *ui;
protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

public slots:
    void Update(int id);
    void OrientationControl();
    void PositionControl();
private slots:
    void on_forward_pressed();
    void on_forward_released();
    void on_backward_pressed();
    void on_backward_released();
    void on_lturn_pressed();
    void on_lturn_released();
    void on_rturn_pressed();
    void on_rturn_released();
    void on_lmove_pressed();
    void on_lmove_released();
    void on_rmove_pressed();
    void on_rmove_released();
    void on_orien_ctrl_btn_clicked();
    void on_down_pressed();
    void on_down_released();
    void on_up_pressed();
    void on_up_released();
    void on_pos_ctrl_btn_clicked();
    void on_turnleft_clicked();
    void on_turnright_clicked();
};

#endif // CONTROLVIEW_H
