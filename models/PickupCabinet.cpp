#include "PickupCabinet.h"
#include <QSqlQuery>
#include <QVariant>

bool PickupCabinet::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS PickupCabinet ("
                      "cabinetId INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "location TEXT,"
                      "capacity INTEGER,"
                      "storedPackageIds TEXT)");
}

PickupCabinet PickupCabinet::fromQuery(const QSqlQuery& query) {
    PickupCabinet c;
    c.cabinetId = query.value("cabinetId").toInt();
    c.location = query.value("location").toString();
    c.capacity = query.value("capacity").toInt();
    c.storedPackageIds = query.value("storedPackageIds").toString();
    return c;
}

QList<PickupCabinet> PickupCabinet::getAll() {
    QList<PickupCabinet> list;
    QSqlQuery query("SELECT * FROM PickupCabinet");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

bool PickupCabinet::addCabinet(const QString& location, int capacity) {
    QSqlQuery query;
    query.prepare("INSERT INTO PickupCabinet (location, capacity, storedPackageIds) VALUES (?, ?, '')");
    query.addBindValue(location);
    query.addBindValue(capacity);
    return query.exec();
}

bool PickupCabinet::removeCabinet(int cabinetId) {
    QSqlQuery query;
    query.prepare("DELETE FROM PickupCabinet WHERE cabinetId = ?");
    query.addBindValue(cabinetId);
    return query.exec();
}

bool PickupCabinet::updateCabinetInfo(int cabinetId, const QString& newLocation, int newCapacity) {
    QSqlQuery query;
    query.prepare("UPDATE PickupCabinet SET location = ?, capacity = ? WHERE cabinetId = ?");
    query.addBindValue(newLocation);
    query.addBindValue(newCapacity);
    query.addBindValue(cabinetId);
    return query.exec();
} 