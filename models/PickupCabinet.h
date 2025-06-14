#pragma once
#include <QString>
#include <QList>
#include <QSqlQuery>

class PickupCabinet {
public:
    int cabinetId;
    QString location;
    int capacity;

    static PickupCabinet fromQuery(const QSqlQuery& query) {
        PickupCabinet c;
        c.cabinetId = query.value("cabinetId").toInt();
        c.location = query.value("location").toString();
        c.capacity = query.value("capacity").toInt();
        return c;
    }
}; 