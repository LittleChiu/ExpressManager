#include "PickupCabinetController.h"

bool PickupCabinetController::addCabinet(const QString& location, int capacity) {
    QSqlQuery query;
    query.prepare("INSERT INTO PickupCabinet (location, capacity, storedPackageIds) VALUES (?, ?, '')");
    query.addBindValue(location);
    query.addBindValue(capacity);
    return query.exec();
}

bool PickupCabinetController::removeCabinet(int cabinetId) {
    QSqlQuery query;
    query.prepare("DELETE FROM PickupCabinet WHERE cabinetId = ?");
    query.addBindValue(cabinetId);
    return query.exec();
}

bool PickupCabinetController::updateCabinetInfo(int cabinetId, const QString& newLocation, int newCapacity) {
    QSqlQuery query;
    query.prepare("UPDATE PickupCabinet SET location = ?, capacity = ? WHERE cabinetId = ?");
    query.addBindValue(newLocation);
    query.addBindValue(newCapacity);
    query.addBindValue(cabinetId);
    return query.exec();
}

QList<PickupCabinet> PickupCabinetController::getAllCabinets() {
    QList<PickupCabinet> list;
    QSqlQuery query("SELECT * FROM PickupCabinet");
    while (query.next()) {
        list.append(PickupCabinet::fromQuery(query));
    }
    return list;
}
bool PickupCabinetController::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS PickupCabinet ("
                      "cabinetId INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "location TEXT,"
                      "capacity INTEGER,"
                      "storedPackageIds TEXT)");
}