#include "Package.h"

#include <QVariant>
#include <QDateTime>

bool Package::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS Package ("
                      "packageId INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "volume REAL,"
                      "weight REAL,"
                      "isFragile INTEGER,"
                      "status INTEGER,"
                      "location TEXT,"
                      "storedTime INTEGER,"
                      "expressCompanyId INTEGER,"
                      "recipientId INTEGER,"
                      "expressmanId INTEGER)");
}

Package Package::fromQuery(const QSqlQuery& query) {
    Package pkg;
    pkg.packageId = query.value("packageId").toInt();
    pkg.volume = query.value("volume").toDouble();
    pkg.weight = query.value("weight").toDouble();
    pkg.isFragile = query.value("isFragile").toInt() != 0;
    pkg.status = static_cast<PackageStatus>(query.value("status").toInt());
    pkg.location = query.value("location").toString();
    pkg.storedTime = query.value("storedTime").toLongLong();
    pkg.expressCompanyId = query.value("expressCompanyId").toInt();
    pkg.recipientId = query.value("recipientId").toInt();
    pkg.expressmanId = query.value("expressmanId").toInt();
    return pkg;
}

QList<Package> Package::getAll() {
    QList<Package> list;
    QSqlQuery query("SELECT * FROM Package");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

bool Package::addPackage(const Package& pkg) {
    QSqlQuery query;
    query.prepare("INSERT INTO Package (volume, weight, isFragile, status, location, storedTime, expressCompanyId, recipientId, expressmanId) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(pkg.volume);
    query.addBindValue(pkg.weight);
    query.addBindValue(pkg.isFragile ? 1 : 0);
    query.addBindValue(static_cast<int>(pkg.status));
    query.addBindValue(pkg.location);
    query.addBindValue(pkg.storedTime);
    query.addBindValue(pkg.expressCompanyId);
    query.addBindValue(pkg.recipientId);
    query.addBindValue(pkg.expressmanId);
    return query.exec();
}

bool Package::updateStatus(int packageId, PackageStatus newStatus) {
    QSqlQuery query;
    query.prepare("UPDATE Package SET status = ? WHERE packageId = ?");
    query.addBindValue(static_cast<int>(newStatus));
    query.addBindValue(packageId);
    return query.exec();
}

QList<Package> Package::queryByRecipient(int recipientId) {
    QList<Package> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Package WHERE recipientId = ?");
    query.addBindValue(recipientId);
    query.exec();
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

QList<Package> Package::queryByLocationAndStatus(const QString& location, PackageStatus status) {\
    QList<Package> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Package WHERE location = ? AND status = ?");
    query.addBindValue(location);
    query.addBindValue(static_cast<int>(status));
    query.exec();
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
} 