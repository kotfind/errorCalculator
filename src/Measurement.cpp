#include "Measurement.h"

#include "dbMacros.h"

#include <exception>

Measurement::Measurement() {}

void Measurement::createTables() {
    PREPARE_NEW(query, R"(
        CREATE TABLE measurements (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            varId INTEGER REFERENCES variables(id),
            value REAL NOT NULL
        )
    )")
    EXEC(query)
}

Measurement Measurement::get(int id) {
    Measurement m;
    m.id = id;
    m.load();

    return m;
}

Measurement Measurement::getNth(int n, int varId) {
    PREPARE_NEW(query, R"(
        SELECT id
        FROM measurements
        WHERE varId = ?
        ORDER BY id
        LIMIT 1
        OFFSET ?
    )")
    query.addBindValue(varId);
    query.addBindValue(n);
    EXEC(query)

    if (!query.next()) throw std::overflow_error("");

    return get(query.value(0).toInt());
}

QList<double> Measurement::getAll(int varId) {
    PREPARE_NEW(query, R"(
        SELECT value
        FROM measurements
        WHERE varId = ?
    )")
    query.addBindValue(varId);
    EXEC(query)

    QList<double> ans;
    while (query.next()) {
        ans.append(query.value(0).toDouble());
    }
    return ans;
}

int Measurement::count(int varId) {
    PREPARE_NEW(query, R"(
        SELECT COUNT(*)
        FROM measurements
        WHERE varId = ?
    )")
    query.addBindValue(varId);
    EXEC(query)
    query.next();
    return query.value(0).toInt();
}

void Measurement::save() {
    if (id == -1) {
        PREPARE_NEW(query, R"(
            INSERT
            INTO measurements(varId, value)
            VALUES (?, ?)
        )")
        query.addBindValue(varId);
        query.addBindValue(value);
        EXEC(query)

        id = query.lastInsertId().toInt();
    } else {
        PREPARE_NEW(query, R"(
            UPDATE measurements
            SET varId = ?,
                value = ?
            WHERE id = ?
        )")
        query.addBindValue(varId);
        query.addBindValue(value);
        query.addBindValue(id);
        EXEC(query)
    }
}

void Measurement::load() {
    PREPARE_NEW(query, R"(
        SELECT varId, value
        FROM measurements
        WHERE id = ?
    )")
    query.addBindValue(id);
    EXEC(query)
    if (!query.next()) throw std::runtime_error("Invalid id");

    varId = query.value(0).toInt();
    value = query.value(1).toDouble();
}

void Measurement::remove() {
    PREPARE_NEW(query, R"(
        DELETE
        FROM measurements
        WHERE id = ?
    )")
    query.addBindValue(id);
    EXEC(query)
}
