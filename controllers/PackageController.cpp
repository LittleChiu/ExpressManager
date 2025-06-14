#include "PackageController.h"

bool PackageController::addPackage(const Package &pkg) {
    QSqlQuery query;
    query.prepare(
        "INSERT INTO Package (volume, isFragile, status, location, storedTime, expressCompanyId, recipientId, expressmanId, pickupCode) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(pkg.volume);
    query.addBindValue(pkg.isFragile ? 1 : 0);
    query.addBindValue(static_cast<int>(pkg.status));
    query.addBindValue(pkg.location);
    query.addBindValue(pkg.storedTime);
    query.addBindValue(pkg.expressCompanyId);
    query.addBindValue(pkg.recipientId);
    query.addBindValue(pkg.expressmanId);
    query.addBindValue(pkg.pickupCode);
    return query.exec();
}

bool PackageController::updateStatus(int packageId, PackageStatus newStatus) {
    QSqlQuery query;
    query.prepare("UPDATE Package SET status = ? WHERE packageId = ?");
    query.addBindValue(static_cast<int>(newStatus));
    query.addBindValue(packageId);
    return query.exec();
}

bool PackageController::updatePickupTime(int packageId, qint64 pickupTime) {
    QSqlQuery query;
    query.prepare("UPDATE Package SET pickupTime = ? WHERE packageId = ?");
    query.addBindValue(pickupTime);
    query.addBindValue(packageId);
    return query.exec();
}

QList<Package> PackageController::getAllPackages() {
    QList<Package> list;
    QSqlQuery query("SELECT * FROM Package");
    while (query.next()) {
        list.append(Package::fromQuery(query));
    }
    return list;
}

QList<Package> PackageController::queryPackagesByRecipient(int recipientId) {
    QList<Package> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Package WHERE recipientId = ?");
    query.addBindValue(recipientId);
    query.exec();
    while (query.next()) {
        list.append(Package::fromQuery(query));
    }
    return list;
}

QList<Package> PackageController::queryByLocationAndStatus(const QString &location, PackageStatus status) {
    QList<Package> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Package WHERE location = ? AND status = ?");
    query.addBindValue(location);
    query.addBindValue(static_cast<int>(status));
    query.exec();
    while (query.next()) {
        list.append(Package::fromQuery(query));
    }
    return list;
}

bool PackageController::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS Package ("
        "packageId INTEGER PRIMARY KEY AUTOINCREMENT,"
        "volume REAL,"
        "weight REAL,"
        "isFragile INTEGER,"
        "status INTEGER,"
        "location TEXT,"
        "storedTime INTEGER,"
        "pickupTime INTEGER DEFAULT -1,"
        "expressCompanyId INTEGER,"
        "recipientId INTEGER,"
        "expressmanId INTEGER,"
        "pickupCode TEXT"
        ")");
}
