#include "ResultsWidget.h"

#include "Variable.h"
#include "Measurement.h"
#include "MeasurementModel.h"
#include "Config.h"
#include "MeasurementProcessor.h"
#include "MeasurementResultsModel.h"
#include "FunctionProcessor.h"
#include "FunctionResultsModel.h"
#include "Function.h"

#include <QVBoxLayout>
#include <QTableView>

ResultsWidget::ResultsWidget(QWidget* parent) : QWidget(parent) {
    setWindowTitle(tr("Results"));

    setAttribute(Qt::WA_DeleteOnClose);

    ui();

    countMeasurementResults();
    measurementResultsModel = new MeasurementResultsModel(
        varNames,
        measurementResults,
        this
    );
    measurementResultView->setModel(measurementResultsModel);

    countFunctionResults();
    functionResultsModel = new FunctionResultsModel(
        functionNames,
        functionResults,
        this
    );
    functionResultView->setModel(functionResultsModel);
}

void ResultsWidget::countMeasurementResults() {
    for (int i = 0; i < Variable::count(); ++i) {
        auto var = Variable::getNth(i);

        MeasurementProcessor proc;
        proc.process(Measurement::getAll(var.getId()), Config::getConf());

        varNames.append(var.name);
        measurementResults.append(proc.getResults());
    }
}

void ResultsWidget::countFunctionResults() {
    for (int i = 0; i < Function::count(); ++i) {
        auto func = Function::getNth(i);

        FunctionProcessor proc;
        proc.process(func.formula, varNames, measurementResults);

        functionNames.append(func.name);
        functionResults.append(proc.getResults());
    }
}

void ResultsWidget::ui() {
    auto* vbox = new QVBoxLayout(this);
    setLayout(vbox);

    measurementResultView = new QTableView;
    vbox->addWidget(measurementResultView);

    functionResultView = new QTableView;
    vbox->addWidget(functionResultView);
}
