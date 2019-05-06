#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <Matrix.h>

class Client : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *m_pTcpSocket;
public:
    explicit Client(const QString &hostName, quint16 port, QObject *parent = nullptr);
    void send(quint16 column, const Matrix &mtx);
signals:
    void received(Matrix mtx);
public slots:
    void slot_connected();
    void slot_readyRead();
    void slot_error(QAbstractSocket::SocketError error);
};

#endif // CLIENT_H
