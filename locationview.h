#ifndef LOCATIONVIEW_H
#define LOCATIONVIEW_H

#include <QWidget>
#include <localization.h>

namespace Ui {
class LocationView;
}

class LocationView : public QWidget
{
    Q_OBJECT

public:
    explicit LocationView(QWidget *parent = 0);
    ~LocationView();

    Localization m_loc;

private:
    Ui::LocationView *ui;

public slots:
    void Update(int id);

};

#endif // LOCATIONVIEW_H
