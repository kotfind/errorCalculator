#include "Variable.h"

#include "dbMacros.h"

#include <cassert>
#include <exception>

Variable::Variable() {}

void Variable::createTables() {
    PREPARE_NEW(query, R"(
        CREATE TABLE variables(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            absError REAL NOT NULL
        )
    )")
    EXEC(query)
}

Variable Variable::get(int id) {
    Variable var;
    var.id = id;
    var.load();
    return var;
}

Variable Variable::getNth(int n) {
    PREPARE_NEW(query, R"(
        SELECT id
        FROM variables
        ORDER BY id
        LIMIT 1
        OFFSET ?
    )")
    query.addBindValue(n);
    EXEC(query)

    if (!query.next()) throw std::overflow_error("");

    return get(query.value(0).toInt());
}

Variable Variable::getBtName(const QString &name) {
    PREPARE_NEW(query, R"(
        SELECT id
        FROM variables
        WHERE name = ?
    )")
    query.addBindValue(name);
    EXEC(query)

    if (!query.next()) throw std::runtime_error("Invalid name");

    return get(query.value(0).toInt());
}

int Variable::count() {
    PREPARE_NEW(query, R"(
        SELECT COUNT(*)
        FROM variables
    )")
    EXEC(query)
    query.next();
    return query.value(0).toInt();
}

void Variable::save() {
    if (id == -1) {
        PREPARE_NEW(query, R"(
            INSERT
            INTO variables(name, absError)
            VALUES (?, ?)
        )")
        query.addBindValue(name);
        query.addBindValue(absError);
        EXEC(query)

        id = query.lastInsertId().toInt();
    } else {
        PREPARE_NEW(query, R"(
            UPDATE variables
            SET name = ?,
                absError = ?
            WHERE id = ?
        )")
        query.addBindValue(name);
        query.addBindValue(absError);
        query.addBindValue(id);
        EXEC(query)
    }
}

void Variable::load() {
    assert(id != -1);

    PREPARE_NEW(query, R"(
        SELECT name, absError
        FROM variables
        WHERE id = ?
    )")
    query.addBindValue(id);
    EXEC(query)

    if (!query.next()) throw std::runtime_error("Invalid id");

    name = query.value(0).toString();
    absError = query.value(1).toDouble();
}

void Variable::remove() {
    assert(id != -1);

    PREPARE_NEW(query, R"(
        DELETE
        FROM variables
        WHERE id = ?
    )")
    query.addBindValue(id);
    EXEC(query)
}
