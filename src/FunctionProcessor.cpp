#include "FunctionProcessor.h"

#include "MathExp.h"
#include "MeasurementProcessor.h"

#include <QString>
#include <math.h>

void FunctionProcessor::process(
    const QString& func,
    const QList<QString>& varNames,
    const QList<MeasurementResults>& varRes
) {
    // Reorganize measurementResults
    for (int i = 0; i < varNames.size(); ++i) {
        meanValues[varNames[i].toStdString()] = varRes[i].mean;
        absErrs[varNames[i].toStdString()] = varRes[i].absErr;
    }

    // Parse func
    exp = new MathExp(func.toStdString()); // TODO catch bad parse

    countMean();
    countGrad();
    countAbsErr();
    countRelErr();

    // Clean up
    delete exp;
}

void FunctionProcessor::countMean() {
    res.mean = exp->calc(meanValues);
}

void FunctionProcessor::countGrad() {
    grad = exp->gradient(meanValues);
}

void FunctionProcessor::countAbsErr() {
    double sum = 0;
    for (const auto& varName : exp->getVars()) {
        sum += abs(grad[varName]) * absErrs[varName];
    }
    res.absErr = sum;
}

void FunctionProcessor::countRelErr() {
    res.relErr = res.absErr / res.mean;
}
