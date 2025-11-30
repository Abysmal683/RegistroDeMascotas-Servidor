#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpservermanager.h"
#include "Mascota.h"
class ControlWidget;

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
    void onRequestUpdateMascota(const Mascota& m);
    void onRequestDeleteMascota(int id);


private:
    Ui::MainWindow *ui;
    ControlWidget *controlWidget;
    TcpServerManager *tcpServerManager;
    int selectPuerto();
};

#endif // MAINWINDOW_H
