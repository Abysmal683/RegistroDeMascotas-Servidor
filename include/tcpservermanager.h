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
    void sendToClient(QTcpSocket* client, const QJsonObject& obj);
    QList<QTcpSocket*> getClients();

signals:
    // Cliente envió datos en bruto (si mantienes un canal genérico)
    void rawMessageReceived(const QByteArray &msg);

    // Peticiones desde el cliente
    void requestAddMascota(const Mascota &m);
    void requestUpdateMascota(const Mascota &m);
    void requestDeleteMascota(int id);
    void requestAllMascotas(QTcpSocket* client);   // cliente pide tabla completa

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
