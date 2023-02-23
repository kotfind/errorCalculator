#include "MeasurementProcessor.h"

#include "stats.h"

#include <math.h>
#include <QDebug>

void MeasurementProcessor::process(const QList<double>& rawValues, double c) {
    res.misses.clear();
    res.values = rawValues;
    conf = c;

    // Calculate
    bool missesFound;
    do {
        countMean();
        countDeltas();
        countSd();
        missesFound = filterMisses();
    } while(missesFound);

    res.absErr = res.sd * getStudentTValue(res.values.size(), conf);
    res.relErr = res.absErr / res.mean;
}

void MeasurementProcessor::countMean() {
    double sum = 0;
    for (auto x : res.values) {
        sum += x;
    }
    res.mean = sum / res.values.size();
}

void MeasurementProcessor::countDeltas() {
    int n = res.values.size();
    deltas.resize(n);
    for (int i = 0; i < n; ++i) {
        deltas[i] = res.values[i] - res.mean;
    }
}

void MeasurementProcessor::countSd() {
    int n = res.values.size();
    double sqDeltaSum = 0;
    for (auto d : deltas) {
        sqDeltaSum += d * d;
    }
    res.sd = sqrt(sqDeltaSum / (n - 1));
}

bool MeasurementProcessor::filterMisses() {
    bool missesFound = false;
    int i = 0;
    while (i < res.values.size()) {
        if (abs(deltas[i]) > 3 * res.sd) {
            missesFound = true;
            res.misses.append(res.values[i]);

            // Delete from res.values
            std::swap(res.values[i], res.values.back());
            res.values.pop_back();

            // Delete from deltas
            std::swap(deltas[i], deltas.back());
            deltas.pop_back();
        } else {
            ++i;
        }
    }
    return missesFound;
}

double MeasurementProcessor::getStudentTValue(int n, double conf) {
    return AStudentT(n - 1, 1 - conf);
}
