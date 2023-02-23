#include "MeasurementResultsModel.h"

#include "Config.h"
#include "MeasurementProcessor.h"

MeasurementResultsModel::MeasurementResultsModel(
    const QList<QString>& varNames,
    const QList<MeasurementResults>& res,
    QObject* parent
) : 
    QAbstractListModel(parent),
    varNames(varNames),
    res(res)
{}

int MeasurementResultsModel::rowCount(const QModelIndex& parent) const {
    return varNames.size();
}

int MeasurementResultsModel::columnCount(const QModelIndex& parent) const {
    return 7;
}

QVariant MeasurementResultsModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole) return QVariant();

    const QString& name = varNames[index.row()];
    const auto& r = res[index.row()];
    switch (index.column()) {
        case 0: return name;
        case 1: return r.mean;
        case 2: return r.absErr;
        case 3: return r.relErr;
        case 4: return r.sd;
        case 5: 
        case 6: {
            const auto& values = index.column() == 5
                ? r.values
                : r.misses;
            if (values.empty()) return "{}";
            QString ans = "{";
            for (auto val : values) {
                ans.append(QString("%1, ").arg(val));
            }
            ans.chop(2); // remove last ", "
            ans += "}";
            return ans;
        }
        default: return QVariant();
    }
}

QVariant MeasurementResultsModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return QVariant();

    switch (section) {
        case 0: return tr("Variable");
        case 1: return tr("Mean");
        case 2: return tr("Abs. Err.");
        case 3: return tr("Rel. Err.");
        case 4: return tr("Std. der.");
        case 5: return tr("Values");
        case 6: return tr("Misses");
        default: return QVariant();
    }
}
