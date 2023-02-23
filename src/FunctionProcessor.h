#pragma once

#include <map>
#include <string>
#include <QList>

class QString;
class MathExp;
class MeasurementResults;

struct FunctionResults {
    double mean;
    double absErr;
    double relErr;
};

class FunctionProcessor {
    public:
        FunctionProcessor() {}

        void process(
            const QString& func,
            const QList<QString>& varNames,
            const QList<MeasurementResults>& varRes
        );

        FunctionResults getResults() const { return res; }

    private:
        void countMean();
        void countGrad();
        void countAbsErr();
        void countRelErr();

        std::map<std::string, double> meanValues;
        std::map<std::string, double> absErrs;
        std::map<std::string, double> grad;

        FunctionResults res;

        MathExp* exp;
};
