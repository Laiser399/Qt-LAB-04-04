#ifndef MATRIXTABLEMODEL_H
#define MATRIXTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <Matrix.h>

class MatrixTableModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    Matrix mtx;
public:
    explicit MatrixTableModel(QObject *parent = nullptr);
    void setMatrix(const Matrix &newMtx);
    Matrix getMatrix() const;

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:
    void slot_appendRow();
    void slot_removeLastRow();
    void slot_appendColumn();
    void slot_removeLastColumn();
};

#endif // MATRIXTABLEMODEL_H
