#pragma once
#include <QString>
#include <QList>
#include "enums.h"
#include <QSqlQuery>

class Package {
public:
    int packageId;
    double volume;
    double weight;
    bool isFragile;
    PackageStatus status;
    QString location;
    qint64 storedTime;
    int expressCompanyId;
    int recipientId;
    int expressmanId;

    static Package fromQuery(const QSqlQuery& query) {
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
}; 