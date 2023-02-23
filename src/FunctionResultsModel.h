#pragma once

#include <QAbstractListModel>
#include <QList>

class FunctionResults;
class QString;

class FunctionResultsModel : public QAbstractListModel {
    public:
        FunctionResultsModel(
            const QList<QString>& funcNames,
            const QList<FunctionResults>& res,
            QObject* parent = nullptr
        );

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    private:
        QList<QString> funcNames;
        QList<FunctionResults> res;
};
