#ifndef TCPSERVERMANAGER_H
#define TCPSERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "Mascota.h"
class TcpServerManager : public QObject
{
    Q_OBJECT

public:
    explicit TcpServerManager(QObject *parent = nullptr);

    bool startServer(quint16 port);

signals:
    // Cliente envió datos en bruto (si mantienes un canal genérico)
    void rawMessageReceived(const QByteArray &msg);

    // Peticiones desde el cliente
    void requestAddMascota(const Mascota &m);
    void requestUpdateMascota(const Mascota &m);
    void requestDeleteMascota(int id);
    void requestAllMascotas();   // cliente pide tabla completa

    // Respuestas que el servidor envía a los clientes
    void sendMascotaAdded(const Mascota &m);
    void sendMascotaUpdated(const Mascota &m);
    void sendMascotaDeleted(int id);
    void sendAllMascotas(const QVector<Mascota> &lista);

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket*> clientes;
    void enviarTodos(const QByteArray &data);
};

#endif // TCPSERVERMANAGER_H
