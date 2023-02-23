#include "MeasurementModel.h"

#include "Measurement.h"
#include "db.h"
#include "GlobalSignalHandler.h"

#include <QDebug>

MeasurementModel::MeasurementModel(QObject* parent) : QAbstractListModel(parent) {
    connect(
        gsh,
        &GlobalSignalHandler::beginResetDb,
        this,
        &MeasurementModel::beginResetModel
    );
    connect(
        gsh,
        &GlobalSignalHandler::endResetDb,
        this,
        &MeasurementModel::endResetModel
    );
}

void MeasurementModel::setVarId(int id) {
    beginResetModel();
    varId = id;
    endResetModel();
}

int MeasurementModel::rowCount(const QModelIndex&) const {
    return db::isOpened() && varId != -1 ? Measurement::count(varId) + 1 : 0;
}

int MeasurementModel::columnCount(const QModelIndex&) const {
    return 1;
}

QVariant MeasurementModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    if (index.row() + 1 == rowCount()) return QVariant();

    auto m = Measurement::getNth(index.row(), varId);

    switch (index.column()) {
        case 0: return m.value;
        default: return QVariant();
    }
}

QVariant MeasurementModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    if (orientation == Qt::Vertical) {
        return section + 1 == rowCount() ? "+" : "";
    }

    switch (section) {
        case 0: return tr("Value");
        default: return QVariant();
    }
}

Qt::ItemFlags MeasurementModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsEditable | QAbstractListModel::flags(index);
}

bool MeasurementModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (index.row() + 1 == rowCount()) {
        insertRow(rowCount());
    }

    auto m = Measurement::getNth(index.row(), varId);

    if (value.toString().trimmed().isEmpty()) {
        removeRow(index.row());
        return true;
    }

    switch (index.column()) {
        case 0:
            if (!value.canConvert<double>()) return false;
            m.value = value.toDouble();
            break;

        default:
            return false;
    }
    m.save();
    return true;
}

bool MeasurementModel::insertRows(int row, int count, const QModelIndex& parent) {
    if (parent.isValid() || count != 1 || row != rowCount()) return false;

    beginInsertRows(QModelIndex(), row, row);

    Measurement m;
    m.value = 0;
    m.varId = varId;
    m.save();

    endInsertRows();

    return true;
}

bool MeasurementModel::removeRows(int row, int count, const QModelIndex& parent) {
    if (parent.isValid() || count != 1) return false;

    beginRemoveRows(QModelIndex(), row, row);

    auto m = Measurement::getNth(row, varId);
    m.remove();

    endRemoveRows();

    return true;
}
