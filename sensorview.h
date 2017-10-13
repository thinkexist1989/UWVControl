#ifndef SENSORVIEW_H
#define SENSORVIEW_H

#include <QWidget>

namespace Ui {
class SensorView;
}

class SensorView : public QWidget
{
    Q_OBJECT

public:
    explicit SensorView(QWidget *parent = 0);
    ~SensorView();

private:
    Ui::SensorView *ui;

public slots:
    void Update(int id);
private slots:
    void on_temp_check_btn_clicked();
    void on_ligh_set_btn_1_clicked();
    void on_ligh_set_btn_2_clicked();
};

#endif // SENSORVIEW_H
