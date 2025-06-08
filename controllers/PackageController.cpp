#include "PackageController.h"

bool PackageController::addPackage(const Package& pkg) {
    return Package::addPackage(pkg);
}

bool PackageController::updateStatus(int packageId, PackageStatus newStatus) {
    return Package::updateStatus(packageId, newStatus);
}

QList<Package> PackageController::getAllPackages() {
    return Package::getAll();
}

QList<Package> PackageController::queryPackagesByRecipient(int recipientId) {
    return Package::queryByRecipient(recipientId);
}

QList<Package> PackageController::queryByLocationAndStatus(const QString& location, PackageStatus status) {
    return Package::queryByLocationAndStatus(location, status);
} 