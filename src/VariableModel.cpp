#include "VariableModel.h"

#include "Variable.h"
#include "db.h"
#include "GlobalSignalHandler.h"

#include <QDebug>

VariableModel::VariableModel(QObject* parent) : QAbstractListModel(parent) {
    connect(
        gsh,
        &GlobalSignalHandler::beginResetDb,
        this,
        &VariableModel::beginResetModel
    );
    connect(
        gsh,
        &GlobalSignalHandler::endResetDb,
        this,
        &VariableModel::endResetModel
    );
}

int VariableModel::rowCount(const QModelIndex&) const {
    return db::isOpened() ? Variable::count() + 1 : 0;
}

int VariableModel::columnCount(const QModelIndex&) const {
    return 2;
}

QVariant VariableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    if (index.row() == rowCount() - 1) return QVariant();

    auto var = Variable::getNth(index.row());

    switch (index.column()) {
        case 0: return var.name;
        case 1: return var.absError;
        default: return QVariant();
    }
}

QVariant VariableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Vertical) {
        return section == rowCount() - 1 ? "+" : "";
    }

    switch (section) {
        case 0: return tr("Name");
        case 1: return tr("Abs. error");
        default: return QVariant();
    }
}

Qt::ItemFlags VariableModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsEditable | QAbstractListModel::flags(index);
}

bool VariableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (index.row() + 1 == rowCount()) {
        insertRow(rowCount());
    }

    auto var = Variable::getNth(index.row());

    if (value.toString().trimmed().isEmpty()) {
        removeRow(index.row());
        return true;
    }

    switch (index.column()) {
        case 0:
            var.name = value.toString();
            break;

        case 1:
            if (!value.canConvert<double>()) return false;
            var.absError = value.toDouble();
            break;

        default:
            return false;
    }
    var.save();
    return true;
}

bool VariableModel::insertRows(int row, int count, const QModelIndex& parent) {
    if (parent.isValid() || count != 1 || row != rowCount()) return false;

    beginInsertRows(QModelIndex(), row, row);

    Variable var;
    var.name = "New";
    var.absError = 0;
    var.save();

    endInsertRows();

    return true;
}

bool VariableModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (parent.isValid() || count != 1) return false;

    beginRemoveRows(QModelIndex(), row, row);

    auto var = Variable::getNth(row);
    var.remove();

    endRemoveRows();

    return true;
}
