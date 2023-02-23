#pragma once

#include <QList>

struct MeasurementResults {
    QList<double> values;   // values without misses
    QList<double> misses;   // misses
    double sd;      // standard deviation
    double mean;    // <x> --- mean value
    double absErr;  // absolute error
    double relErr;  // relative error
};

class MeasurementProcessor {
    public:
        MeasurementProcessor() {}

        void process(const QList<double>& rawValues, double conf);

        MeasurementResults getResults() const { return res; }

    private:
        void countMean();
        void countDeltas();
        void countSd();
        bool filterMisses(); // returns true if misses found
        double getStudentTValue(int n, double conf);

        MeasurementResults res;

        QList<double> deltas;
        double conf;
};
