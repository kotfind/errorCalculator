#pragma once

class QString;

namespace db {
    void openNew(const QString& fileName);

    void open(const QString& fileName, bool emitSignals = true);
    void close();

    bool isOpened();
};
