#pragma once

#include <QList>

class Measurement {
    public:
        Measurement();

        static void createTables();
        static Measurement get(int id);
        static Measurement getNth(int n, int varId);
        static QList<double> getAll(int varId);
        static int count(int varId);

        void save();
        void load();
        void remove();

        int varId;
        double value;

    private:
        int id = -1;
};

