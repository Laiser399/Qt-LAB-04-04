#include "MatrixTableModel.h"

MatrixTableModel::MatrixTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    mtx.resize(1, 1);

}

void MatrixTableModel::setMatrix(const Matrix &newMtx) {
    emit beginResetModel();
    if ((newMtx.rowCount() == 0) || (newMtx.columnCount() == 0)) {
        mtx.resize(1, 1);
        mtx.setValue(0, 0, 0);
    }
    else {
        mtx = newMtx;
    }
    emit endResetModel();
}

Matrix MatrixTableModel::getMatrix() const {
    return mtx;
}

QModelIndex MatrixTableModel::index(int row, int column, const QModelIndex &parent) const
{
    if ((row < 0) || (row >= mtx.rowCount()) || (column < 0) || (column >= mtx.columnCount()))
        return QModelIndex();
    if (parent.isValid())
        return QModelIndex();
    return createIndex(row, column, nullptr);
}

int MatrixTableModel::rowCount(const QModelIndex &parent) const
{
    return mtx.rowCount();
}

int MatrixTableModel::columnCount(const QModelIndex &parent) const
{
    return mtx.columnCount();
}

QVariant MatrixTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();;
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return mtx.getValue(index.row(), index.column());
    default:
        return QVariant();
    }
}

bool MatrixTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    switch (role) {
    case Qt::EditRole:
        mtx.setValue(index.row(), index.column(), value.toDouble());
        return true;
    default:
        return false;
    }
}

Qt::ItemFlags MatrixTableModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    else
        return 0;
}

void MatrixTableModel::slot_appendRow() {
    emit beginInsertRows(QModelIndex(), mtx.rowCount(), mtx.rowCount());
    mtx.resize(mtx.rowCount() + 1, mtx.columnCount());
    emit endInsertRows();
}

void MatrixTableModel::slot_removeLastRow() {
    if (mtx.rowCount() > 1) {
        emit beginRemoveRows(QModelIndex(), mtx.rowCount() - 1, mtx.rowCount() - 1);
        mtx.resize(mtx.rowCount() - 1, mtx.columnCount());
        emit endRemoveRows();
    }
}

void MatrixTableModel::slot_appendColumn() {
    emit beginInsertColumns(QModelIndex(), mtx.columnCount(), mtx.columnCount());
    mtx.resize(mtx.rowCount(), mtx.columnCount() + 1);
    emit endInsertColumns();
}

void MatrixTableModel::slot_removeLastColumn() {
    if (mtx.columnCount() > 1) {
        emit beginRemoveColumns(QModelIndex(), mtx.columnCount() - 1, mtx.columnCount() - 1);
        mtx.resize(mtx.rowCount(), mtx.columnCount() - 1);
        emit endRemoveColumns();
    }
}
