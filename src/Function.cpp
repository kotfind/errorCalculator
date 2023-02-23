#include "Function.h"

#include "dbMacros.h"

#include <cassert>
#include <exception>

Function::Function() {}

void Function::createTables() {
    PREPARE_NEW(query, R"(
        CREATE TABLE functions(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            formula TEXT NOT NULL
        )
    )")
    EXEC(query)
}

Function Function::get(int id) {
    Function f;
    f.id = id;
    f.load();
    return f;
}

Function Function::getNth(int n) {
    PREPARE_NEW(query, R"(
        SELECT id
        FROM functions
        ORDER BY id
        LIMIT 1
        OFFSET ?
    )")
    query.addBindValue(n);
    EXEC(query)

    if (!query.next()) throw std::overflow_error("");

    return get(query.value(0).toInt());
}

int Function::count() {
    PREPARE_NEW(query, R"(
        SELECT COUNT(*)
        FROM functions
    )")
    EXEC(query)
    query.next();
    return query.value(0).toInt();
}

void Function::save() {
    if (id == -1) {
        PREPARE_NEW(query, R"(
            INSERT
            INTO functions(name, formula)
            VALUES (?, ?)
        )")
        query.addBindValue(name);
        query.addBindValue(formula);
        EXEC(query)

        id = query.lastInsertId().toInt();
    } else {
        PREPARE_NEW(query, R"(
            UPDATE functions
            SET name = ?,
                formula = ?
            WHERE id = ?
        )")
        query.addBindValue(name);
        query.addBindValue(formula);
        query.addBindValue(id);
        EXEC(query)
    }
}

void Function::load() {
    assert(id != -1);

    PREPARE_NEW(query, R"(
        SELECT name, formula 
        FROM functions
        WHERE id = ?
    )")
    query.addBindValue(id);
    EXEC(query)

    if (!query.next()) throw std::runtime_error("Invalid id");

    name = query.value(0).toString();
    formula = query.value(1).toString();
}

void Function::remove() {
    assert(id != -1);

    PREPARE_NEW(query, R"(
        DELETE
        FROM functions
        WHERE id = ?
    )")
    query.addBindValue(id);
    EXEC(query)
}
