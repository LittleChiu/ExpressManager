#pragma once
#include <QString>
#include <QList>
#include "enums.h"
#include <QSqlQuery>

class Package {
public:
    int packageId = -1;
    double volume = -1;

    bool isFragile = false;
    PackageStatus status;
    QString location;
    qint64 storedTime = -1;
    int expressCompanyId = -1;
    int recipientId = -1;
    int expressmanId = -1;
    QString pickupCode;
    qint64 pickupTime = -1;

    static Package fromQuery(const QSqlQuery &query) {
        Package pkg;
        pkg.packageId = query.value("packageId").toInt();
        pkg.volume = query.value("volume").toDouble();
        pkg.isFragile = query.value("isFragile").toInt() != 0;
        pkg.status = static_cast<PackageStatus>(query.value("status").toInt());
        pkg.location = query.value("location").toString();
        pkg.storedTime = query.value("storedTime").toLongLong();
        pkg.expressCompanyId = query.value("expressCompanyId").toInt();
        pkg.recipientId = query.value("recipientId").toInt();
        pkg.expressmanId = query.value("expressmanId").toInt();
        pkg.pickupCode = query.value("pickupCode").toString();
        pkg.pickupTime = query.value("pickupTime").toLongLong();
        return pkg;
    }
};
