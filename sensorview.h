#ifndef SENSORVIEW_H
#define SENSORVIEW_H

#include <QWidget>
#include <QFileDialog>

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

namespace Ui {
class SensorView;
}

class SensorView : public QWidget
{
    Q_OBJECT

public:
    explicit SensorView(QWidget *parent = 0);
    ~SensorView();

    std::vector<RECXsensData> xsensdatavec;
    std::vector<float> kellerdatavec;
    std::vector<RECAltData> altdatavec;

private:
    Ui::SensorView *ui;
    bool sensordatarecording;

public slots:
    void Update(int id);
private slots:
    void on_temp_check_btn_clicked();
    void on_ligh_set_btn_1_clicked();
    void on_ligh_set_btn_2_clicked();
    void on_rec_sensor_data_clicked();
};

#endif // SENSORVIEW_H
