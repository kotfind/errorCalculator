#pragma once

#include <QString>

class Variable {
    public:
        Variable();

        static void createTables();
        static Variable get(int id);
        static Variable getNth(int n);
        static Variable getBtName(const QString& name);
        static int count();

        int getId() { return id; }
        void save();
        void load();
        void remove();

        QString name;
        double absError;

    private:
        int id = -1;
};
