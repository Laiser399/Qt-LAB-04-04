#include "Server.h"
#include <QDataStream>
#include <QTime>
#include <QDebug>
#include <Matrix.h>

//   functions   |
int searchColMaxSum(const Matrix &mtx) {
    double max_sum = 0;
    bool first = true;
    int res_column = 0;
    for (int i = 0; i < mtx.columnCount(); i++) {
        double sum = 0;
        for (int j = 0; j < mtx.rowCount(); j++)
            sum += mtx.getValue(j, i);
        if (first || (sum >= max_sum)) {
            max_sum = sum;
            first = false;
            res_column = i;
        }
    }
    return res_column;
}

Matrix makeSubMatrix(quint16 column, const Matrix &mtx) {
    Matrix resMtx;
    if (column >= mtx.columnCount())
        return resMtx;
    resMtx.resize(mtx.rowCount(), column + 1);
    for (int i = 0; i < resMtx.rowCount(); i++) {
        for (int j = 0; j < resMtx.columnCount(); j++) {
            resMtx.setValue(i, j, mtx.getValue(i, j));
        }
    }
    return resMtx;
}

Matrix makeArrayByColumn(quint16 column, const Matrix &mtx) {
    Matrix resMtx;
    if (column >= mtx.columnCount())
        return resMtx;
    resMtx.resize(1, mtx.rowCount());
    for (int i = 0; i < resMtx.columnCount(); i++) {
        resMtx.setValue(0, i, mtx.getValue(i, column));
    }
    return resMtx;
}

void sendToClient(QTcpSocket *clientSocket, const Matrix &mtx) {
    QByteArray arr;
    QDataStream out(&arr, QIODevice::WriteOnly);
    out << quint16(0) << mtx.getStringMtx();
    out.device()->seek(0);
    out << quint16(arr.size() - sizeof(quint16));
    clientSocket->write(arr);
}

//------------|
//   public   |
//------------|
Server::Server(quint16 port, QObject *parent) : QObject(parent),
    m_ptcpServer(new QTcpServer(this))
{
    Q_ASSERT(m_ptcpServer->listen(QHostAddress::Any, port));
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this, SLOT(slot_newConnection()));
}

//-------------------|
//   private slots   |
//-------------------|
void Server::slot_newConnection() {
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slot_readClient()));

}

void Server::slot_readClient() {
    QTcpSocket *pClientSocket = static_cast<QTcpSocket*>(sender());
    quint16 nextBlockSize = pClientSocket->property("next_block_size").toInt();

    QDataStream in(pClientSocket);
    while (true) {
        if (!nextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> nextBlockSize;
            pClientSocket->setProperty("next_block_size", nextBlockSize);
        }

        if (pClientSocket->bytesAvailable() < nextBlockSize) {
            break;
        }

        pClientSocket->setProperty("next_block_size", 0);
        QString mtxStr;
        quint16 column, maxSumCol;
        Matrix mtx, newMtx;
        in >> column >> mtxStr;
        mtx.readFromStr(mtxStr);
        maxSumCol = searchColMaxSum(mtx);
        if (maxSumCol > column) {
            newMtx = makeSubMatrix(maxSumCol, mtx);
            sendToClient(pClientSocket, newMtx);
        }
        else {
            newMtx = makeArrayByColumn(column, mtx);
            sendToClient(pClientSocket, newMtx);
        }

        // send to client

        break;
    }
}
