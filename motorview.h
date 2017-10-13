#ifndef MOTORVIEW_H
#define MOTORVIEW_H

#include <QWidget>

namespace Ui {
class MotorView;
}

class MotorView : public QWidget
{
    Q_OBJECT

public:
    explicit MotorView(QWidget *parent = 0);
    ~MotorView();

private:
    Ui::MotorView *ui;

public slots:
    void Update();
private slots:
    void on_SendButton_clicked();
    void on_pwm_1_valueChanged(double arg1);
    void on_pwm_2_valueChanged(double arg1);
    void on_pwm_3_valueChanged(double arg1);
    void on_pwm_4_valueChanged(double arg1);
    void on_pwm_5_valueChanged(double arg1);
    void on_pwm_6_valueChanged(double arg1);
    void on_pwm_7_valueChanged(double arg1);
    void on_pwm_8_valueChanged(double arg1);
};

#endif // MOTORVIEW_H
