#include "db.h"

#include "GlobalSignalHandler.h"
#include "Variable.h"
#include "Measurement.h"
#include "Function.h"
#include "Config.h"

#include <QFile>
#include <QString>
#include <QSqlDatabase>
#include <exception>
#include <QDebug>

void db::openNew(const QString& fileName) {
    emit gsh->beginResetDb();

    QFile::remove(fileName);
    open(fileName, false);

    /* BEGIN: create tables */
    Variable::createTables();
    Measurement::createTables();
    Function::createTables();
    Config::createTables();
    /* END: create tables */

    emit gsh->endResetDb();
}

void db::open(const QString &fileName, bool emitSignals) {
    if (emitSignals) {
        emit gsh->beginResetDb();
    }

    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fileName);
    if (!db.open()) {
        throw std::runtime_error(
            QString("Cannot open file %1 as database.")
            .arg(fileName)
            .toStdString()
        );
    }

    if (emitSignals) {
        emit gsh->endResetDb();
    }
}

void db::close() {
    if (QSqlDatabase::database().isValid()) {
        emit gsh->beginResetDb();
        QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        emit gsh->endResetDb();
    }
}

bool db::isOpened() {
    return QSqlDatabase::database().isValid();
}
