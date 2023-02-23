#pragma once

class Config {
    public:
        static void createTables();

        static double getConf();
        static void setConf(double);

    private:
        Config() {}
};
