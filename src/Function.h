#pragma once

#include <QString>

class Function {
    public:
        Function();

        static void createTables();
        static Function get(int id);
        static Function getNth(int n);
        static int count();

        int getId() { return id; }
        void save();
        void load();
        void remove();

        QString name;
        QString formula;

    private:
        int id = -1;
};
