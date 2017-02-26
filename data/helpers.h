#ifndef HELPERS_H
#define HELPERS_H
#include <QVariant>
#include <QSqlQuery>

inline QVariant getValueFromDB(QString table, QString field, QVariant value, QString targetField) {
    QSqlQuery query;
    query.prepare("SELECT * FROM " + table + " WHERE " + field + "=:value");
    query.bindValue(":value", value);
    query.exec();
    if(query.next()) {
        return query.value(targetField);
    }
    return QVariant::Invalid;
}

inline QSqlQuery getQueryFromDB(QString table, QString field, QVariant value) {
    QSqlQuery query;
    query.prepare("SELECT * FROM " + table + " WHERE " + field + "=:value");
    query.bindValue(":value", value);
    query.exec();
    return query;
}

#endif // HELPERS_H
