#include "ExpressCompany.h"
#include <QSqlQuery>
#include <QVariant>
#include <QList>

bool ExpressCompany::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS ExpressCompany ("
                      "companyId INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "name TEXT UNIQUE,"
                      "packageFee REAL)");
}

ExpressCompany ExpressCompany::fromQuery(const QSqlQuery& query) {
    ExpressCompany c;
    c.companyId = query.value("companyId").toInt();
    c.name = query.value("name").toString();
    c.packageFee = query.value("packageFee").toDouble();
    return c;
}

QList<ExpressCompany> ExpressCompany::getAll() {
    QList<ExpressCompany> list;
    QSqlQuery query("SELECT * FROM ExpressCompany");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

bool ExpressCompany::addCompany(const QString& name, double fee) {
    QSqlQuery query;
    query.prepare("INSERT INTO ExpressCompany (name, packageFee) VALUES (?, ?)");
    query.addBindValue(name);
    query.addBindValue(fee);
    return query.exec();
}

bool ExpressCompany::removeCompany(int companyId) {
    QSqlQuery query;
    query.prepare("DELETE FROM ExpressCompany WHERE companyId = ?");
    query.addBindValue(companyId);
    return query.exec();
}

bool ExpressCompany::updateFee(int companyId, double newFee) {
    QSqlQuery query;
    query.prepare("UPDATE ExpressCompany SET packageFee = ? WHERE companyId = ?");
    query.addBindValue(newFee);
    query.addBindValue(companyId);
    return query.exec();
} 