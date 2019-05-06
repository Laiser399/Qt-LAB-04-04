#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>

class Matrix
{
private:
    QVector<QVector<double> > data;
public:
    Matrix();
    int columnCount() const;
    int rowCount() const;
    void resize(int rowCount, int columnCount);
    void setValue(int row, int column, double value);
    double getValue(int row, int column) const;
    void readFromStr(const QString &str);
    QString getStringMtx() const;
};

#endif // MATRIX_H
