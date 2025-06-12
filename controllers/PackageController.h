#pragma once
#include <QObject>
#include "Package.h"

class PackageController : public QObject {
    Q_OBJECT
public:
    static PackageController& instance() {
        static PackageController instance;
        return instance;
    }

    static bool createTable();
    bool addPackage(const Package& pkg);
    bool updateStatus(int packageId, PackageStatus newStatus);
    bool updatePickupTime(int packageId, qint64 pickupTime);
    QList<Package> getAllPackages();
    QList<Package> queryPackagesByRecipient(int recipientId);
    QList<Package> queryByLocationAndStatus(const QString& location, PackageStatus status);
}; 