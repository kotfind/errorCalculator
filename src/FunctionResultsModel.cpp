#include "FunctionResultsModel.h"

#include "FunctionProcessor.h"

#include <QString>

FunctionResultsModel::FunctionResultsModel(
    const QList<QString>& funcNames,
    const QList<FunctionResults>& res,
    QObject* parent
) :
    QAbstractListModel(parent),
    funcNames(funcNames),
    res(res)
{}

int FunctionResultsModel::rowCount(const QModelIndex& parent) const {
    return funcNames.size();
}

int FunctionResultsModel::columnCount(const QModelIndex& parent) const {
    return 4;
}

QVariant FunctionResultsModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

    const auto& name = funcNames[index.row()];
    const auto& r = res[index.row()];
    switch (index.column()) {
        case 0: return name;
        case 1: return r.mean;
        case 2: return r.absErr;
        case 3: return r.relErr;
        default: return QVariant();
    }
}

QVariant FunctionResultsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return QVariant();

    switch (section) {
        case 0: return tr("Name");
        case 1: return tr("Mean");
        case 2: return tr("Abs. Err.");
        case 3: return tr("Rel. Err.");
        default: return QVariant();
    }
}
