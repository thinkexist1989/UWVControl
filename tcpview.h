#ifndef TCPVIEW_H
#define TCPVIEW_H

#include <QWidget>
#include <QtNetwork>
#include <QtNetwork/QTcpSocket>
#include <QLineEdit>

namespace Ui {
class TcpView;
}

class TcpView : public QWidget
{
    Q_OBJECT

public:
    explicit TcpView(QWidget *parent = 0);
    ~TcpView();

    void FrameAnalysis(char *recvbuf, int n);


    bool isConnected;

private:
    Ui::TcpView *ui;

    int micnt, mccnt;
    int picnt, pccnt;
    int xcnt, kcnt, acnt;
    int ticnt, tccnt;
    int licnt, lccnt;

private slots:
    void RecvData();
    void DisplayError(QAbstractSocket::SocketError);
    void UpdateUI(QLineEdit* pld, char* buf, int n);
    void on_Btn_Conn_clicked();

signals:
    void recievedmotorinfo();
    void recievedplatforminfo();
    void recievedsensorinfo(int);
};

#endif // TCPVIEW_H
