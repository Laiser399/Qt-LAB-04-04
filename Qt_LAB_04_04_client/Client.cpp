#include "Client.h"
#include <QDataStream>
#include <QTextStream>
#include <QByteArray>

Client::Client(const QString &hostName, quint16 port, QObject *parent) : QObject(parent),
    m_pTcpSocket(new QTcpSocket(this))
{
    m_pTcpSocket->connectToHost(hostName, port);
    connect(m_pTcpSocket, SIGNAL(connected()),
            this, SLOT(slot_connected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()),
            this, SLOT(slot_readyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slot_error(QAbstractSocket::SocketError)));

}

void Client::send(quint16 column, const Matrix &mtx) {
    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out << quint16(0) << column << mtx.getStringMtx();
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));
    m_pTcpSocket->write(arr);
}

void Client::slot_connected() {
    qDebug() << "client: connected";
}

void Client::slot_readyRead() {
    QDataStream in(m_pTcpSocket);
    quint16 nextBlockSize = m_pTcpSocket->property("next_block_size").toUInt();
    while (true) {
        if (!nextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16))
                break;
            in >> nextBlockSize;
            m_pTcpSocket->setProperty("next_block_size", nextBlockSize);
        }

        if (m_pTcpSocket->bytesAvailable() < nextBlockSize)
            break;

        QString str;
        Matrix mtx;
        in >> str;
        mtx.readFromStr(str);
        emit received(mtx);
        m_pTcpSocket->setProperty("next_block_size", 0);
        break;
    }
}

void Client::slot_error(QAbstractSocket::SocketError error) {
    QString strError =
            "error: " + (error == QAbstractSocket::HostNotFoundError ?
                         "The host was not found." :
                         error == QAbstractSocket::RemoteHostClosedError ?
                         "The remote host is closed." :
                         error == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused." :
                         QString(m_pTcpSocket->errorString())
                        );
    qDebug() << "client:" << strError;
}
