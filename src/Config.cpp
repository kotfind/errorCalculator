#include "Config.h"

#include "dbMacros.h"

void Config::createTables() {
    QSqlQuery query;

    PREPARE(query, R"(
        CREATE TABLE config (
            conf REAL -- Confidence coefficient
        )
    )")
    EXEC(query)
    query.finish();

    PREPARE(query, R"(
        INSERT INTO config (conf) VALUES (0.95)
    )")
    EXEC(query)
    query.finish();
}

double Config::getConf() {
    PREPARE_NEW(query, R"(
        SELECT conf
        FROM config
    )")
    EXEC(query)
    query.next();
    return query.value(0).toDouble();
}

void Config::setConf(double v) {
    PREPARE_NEW(query, R"(
        UPDATE config
        SET conf = ?
    )")
    query.addBindValue(v);
    EXEC(query)
}
