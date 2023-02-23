#pragma once

#include <QObject>

class GlobalSignalHandler : public QObject {
    Q_OBJECT

    public:
        static GlobalSignalHandler* instance;

    private:
        GlobalSignalHandler();

    signals:
        void beginResetDb();
        void endResetDb();
};

extern GlobalSignalHandler* gsh; // Same as GlobalSignalHandler::instance
