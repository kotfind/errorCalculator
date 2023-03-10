#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>

class MeasurementResults;

class MeasurementResultsModel : public QAbstractListModel {
    public:
        MeasurementResultsModel(
            const QList<QString>& varNames,
            const QList<MeasurementResults>& res,
            QObject* parent = nullptr
        );

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    private:
        QList<QString> varNames;
        QList<MeasurementResults> res;
};
