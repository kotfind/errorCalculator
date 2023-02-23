#include "FunctionModel.h"

#include "Function.h"
#include "db.h"
#include "GlobalSignalHandler.h"

#include <QDebug>

FunctionModel::FunctionModel(QObject* parent) : QAbstractListModel(parent) {
    connect(
        gsh,
        &GlobalSignalHandler::beginResetDb,
        this,
        &FunctionModel::beginResetModel
    );
    connect(
        gsh,
        &GlobalSignalHandler::endResetDb,
        this,
        &FunctionModel::endResetModel
    );
}

int FunctionModel::rowCount(const QModelIndex&) const {
    return db::isOpened() ? Function::count() + 1 : 0;
}

int FunctionModel::columnCount(const QModelIndex&) const {
    return 2;
}

QVariant FunctionModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    if (index.row() == rowCount() - 1) return QVariant();

    auto f = Function::getNth(index.row());

    switch (index.column()) {
        case 0: return f.name;
        case 1: return f.formula;
        default: return QVariant();
    }
}

QVariant FunctionModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Vertical) {
        return section == rowCount() - 1 ? "+" : "";
    }

    switch (section) {
        case 0: return tr("Name");
        case 1: return tr("Formula");
        default: return QVariant();
    }
}

Qt::ItemFlags FunctionModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsEditable | QAbstractListModel::flags(index);
}

bool FunctionModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (index.row() + 1 == rowCount()) {
        insertRow(rowCount());
    }

    auto f = Function::getNth(index.row());

    if (value.toString().trimmed().isEmpty()) {
        removeRow(index.row());
        return true;
    }

    switch (index.column()) {
        case 0:
            f.name = value.toString();
            break;

        case 1:
            f.formula = value.toString();
            break;

        default:
            return false;
    }
    f.save();
    return true;
}

bool FunctionModel::insertRows(int row, int count, const QModelIndex& parent) {
    if (parent.isValid() || count != 1 || row != rowCount()) return false;

    beginInsertRows(QModelIndex(), row, row);

    Function f;
    f.name = "New";
    f.formula = "0";
    f.save();

    endInsertRows();

    return true;
}

bool FunctionModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (parent.isValid() || count != 1) return false;

    beginRemoveRows(QModelIndex(), row, row);

    auto f = Function::getNth(row);
    f.remove();

    endRemoveRows();

    return true;
}
