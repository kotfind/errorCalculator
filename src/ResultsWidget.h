#pragma once

#include <QWidget>
#include <QList>

class MeasurementResults;
class MeasurementResultsModel;
class FunctionResults;
class FunctionResultsModel;
class QTableView;
class QString;

class ResultsWidget : public QWidget {
    public:
        ResultsWidget(QWidget* parent = nullptr);

    private:
        void ui();

        void countMeasurementResults();
        void countFunctionResults();

        QList<QString> varNames;
        QList<MeasurementResults> measurementResults;
        MeasurementResultsModel* measurementResultsModel;
        QTableView* measurementResultView;

        QList<QString> functionNames;
        QList<FunctionResults> functionResults;
        FunctionResultsModel* functionResultsModel;
        QTableView* functionResultView;
};
