#pragma once

#include <QAbstractListModel>

class MeasurementModel : public QAbstractListModel {
    public:
        MeasurementModel(QObject* parent = nullptr);

        void setVarId(int id);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

        bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
        bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    private:
        int varId = -1;
};
