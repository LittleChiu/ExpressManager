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


    bool addPackage(const Package& pkg);
    bool updateStatus(int packageId, PackageStatus newStatus);
    QList<Package> getAllPackages();
    QList<Package> queryPackagesByRecipient(int recipientId);
    QList<Package> queryByLocationAndStatus(const QString& location, PackageStatus status);
}; 