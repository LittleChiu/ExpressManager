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

    static bool createTable();
    static Package fromQuery(const QSqlQuery& query);
    static QList<Package> getAll();
    static bool addPackage(const Package& pkg);
    static bool updateStatus(int packageId, PackageStatus newStatus);
    static QList<Package> queryByRecipient(int recipientId);
    static QList<Package> queryByLocationAndStatus(const QString& location, PackageStatus status);
}; 