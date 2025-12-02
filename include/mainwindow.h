#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "mascota.h"
class ControlWidget;
class TcpServerManager;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onRequestAllMascotas(QTcpSocket* client);
    void onRequestAddMascota(QTcpSocket* client,const Mascota& m);
    void onRequestUpdateMascota(QTcpSocket* client,const Mascota& m);
    void onRequestDeleteMascota(QTcpSocket* client,int id);
    void onNewClient(QString msg);
    void onDisconnectClient(QString msg);
    void onRequestResearchIdMascota(QTcpSocket* client,int id);
    void onRequestViewImagenMascota(QTcpSocket* client,int id);
    void onRequestResearchNameMascota(QTcpSocket* client,QString name);

private:
    Ui::MainWindow *ui;
    ControlWidget *controlWidget;
    TcpServerManager *tcpServerManager;
    int selectPuerto();
};

#endif // MAINWINDOW_H
