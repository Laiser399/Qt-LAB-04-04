#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
private:
    QTcpServer *m_ptcpServer;
    //int m_nNextBlockSize;
public:
    explicit Server(quint16 port, QObject *parent = nullptr);

signals:

private slots:
    void slot_newConnection();
    void slot_readClient();
};

#endif // SERVER_H
