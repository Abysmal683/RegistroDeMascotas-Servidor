#ifndef TCPSERVERMANAGER_H
#define TCPSERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "mascota.h"
class TcpServerManager : public QObject
{
    Q_OBJECT

public:
    explicit TcpServerManager(QObject *parent = nullptr);
    bool startServer(quint16 port);
    void sendToClient(QTcpSocket* client, const QJsonObject& obj);
    QList<QTcpSocket*> getClients();
signals:
    // Peticiones desde el cliente
    void requestAddMascota(QTcpSocket* client,const Mascota &m);
    void requestUpdateMascota(const Mascota &m);
    void requestDeleteMascota(int id);
    void requestResearchIdMascota(QTcpSocket* client,int id);
    void requestResearchNameMascota(QTcpSocket* client,QString name);
    void requestAllMascotas(QTcpSocket* client);   // cliente pide tabla completa
    void newClientConnect(QString msg);
    void clientDisconnect(QString msg);
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
