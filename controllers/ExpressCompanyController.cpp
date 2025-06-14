#include "ExpressCompanyController.h"

bool ExpressCompanyController::addCompany(const QString &name, double fee) {
    QSqlQuery query;
    query.prepare("INSERT INTO ExpressCompany (name, packageFee) VALUES (?, ?)");
    query.addBindValue(name);
    query.addBindValue(fee);
    return query.exec();
}

bool ExpressCompanyController::removeCompany(int companyId) {
    QSqlQuery query;
    query.prepare("DELETE FROM ExpressCompany WHERE companyId = ?");
    query.addBindValue(companyId);
    return query.exec();
}

bool ExpressCompanyController::updateFee(int companyId, double newFee) {
    QSqlQuery query;
    query.prepare("UPDATE ExpressCompany SET packageFee = ? WHERE companyId = ?");
    query.addBindValue(newFee);
    query.addBindValue(companyId);
    return query.exec();
}

QList<ExpressCompany> ExpressCompanyController::getAllCompanies() {
    QList<ExpressCompany> list;
    QSqlQuery query("SELECT * FROM ExpressCompany");
    while (query.next()) {
        list.append(ExpressCompany::fromQuery(query));
    }
    return list;
}

bool ExpressCompanyController::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS ExpressCompany ("
        "companyId INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT UNIQUE,"
        "packageFee REAL)");
}
