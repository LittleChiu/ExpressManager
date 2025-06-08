#pragma once
#include <QString>
#include <QList>
#include <QSqlQuery>

class PickupCabinet {
public:
    int cabinetId;
    QString location;
    int capacity;
    QString storedPackageIds; // 用逗号分隔的包裹ID字符串

    static bool createTable();
    static PickupCabinet fromQuery(const QSqlQuery& query);
    static QList<PickupCabinet> getAll();
    static bool addCabinet(const QString& location, int capacity);
    static bool removeCabinet(int cabinetId);
    static bool updateCabinetInfo(int cabinetId, const QString& newLocation, int newCapacity);
}; 