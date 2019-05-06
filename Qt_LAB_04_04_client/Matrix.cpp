#include "Matrix.h"
#include <QTextStream>
#include <QDebug>

Matrix::Matrix()
{

}

int Matrix::columnCount() const{
    if (data.size() == 0)
        return 0;
    else
        return data[0].size();
}

int Matrix::rowCount() const {
    return data.size();
}

void Matrix::resize(int rowCount, int columnCount) {
    if ((rowCount < 0) || (columnCount < 0))
        return;
    if ((rowCount == 0) || (columnCount == 0)) {
        data.resize(0);
        return;
    }
    data.resize(rowCount);
    for (int i = 0; i < data.size(); i++)
        data[i].resize(columnCount);
}

void Matrix::setValue(int row, int column, double value) {
    if ((row >= 0) && (row < data.size())
         && (column >= 0) && (column < data[row].size()))
    {
        data[row][column] = value;
    }
}

double Matrix::getValue(int row, int column) const {
    if ((row >= 0) && (row < data.size())
         && (column >= 0) && (column < data[row].size()))
    {
        return data[row][column];
    }
    else {
        throw -1;
    }
}

void Matrix::readFromStr(const QString &str) {
    data.clear();
    QStringList rows = str.split('|');
    data.resize(rows.size());
    int max_height = 0;
    for (int i = 0; i < rows.size(); ++i) {
        QTextStream stream(&rows[i]);
        double val;
        while (true) {
            stream >> val;
            if (stream.status() == QTextStream::Ok)
                data[i].append(val);
            else {
                if (data[i].size() > max_height)
                    max_height = data[i].size();
                break;
            }
        }
    }
    // alignment rows
    for (int i = 0; i < data.size(); i++) {
        data[i].resize(max_height);
    }
}

QString Matrix::getStringMtx() const {
    QString res;
    if ((data.size() == 0) || (data[0].size() == 0))
        return res;
    for (int i = 0; i < data.size() - 1; i++) {
        for (int j = 0; j < data[i].size() - 1; j++) {
            res += QString::number(data[i][j]) + ' ';
        }
        res += QString::number(data[i].last()) + '|';
    }
    for (int j = 0; j < data.last().size() - 1; j++) {
        res += QString::number(data.last()[j]) + ' ';
    }
    res += QString::number(data.last().last());

    return res;
}

