#pragma once

#include <exception>
#include <QSqlQuery>
#include <QSqlError>

#define PREPARE(query, str) \
if (!(query).prepare(str)) { \
    throw std::runtime_error(QString("PREPARE FAILED at %1(%2): %3")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        .arg((query).lastError().text()).toStdString()); \
} \
(query).setForwardOnly(true);

#define PREPARE_NEW(query, str) \
QSqlQuery query; \
PREPARE(query, str)

#define EXEC(query) \
if (!(query).exec()) { \
    throw std::runtime_error(QString("EXEC FAILED at %1(%2): %3")\
        .arg(__FILE__) \
        .arg(__LINE__) \
        .arg((query).lastError().text()).toStdString()); \
}
