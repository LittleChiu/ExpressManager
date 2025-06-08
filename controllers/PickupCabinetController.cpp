#include "PickupCabinetController.h"

bool PickupCabinetController::addCabinet(const QString& location, int capacity) {
    return PickupCabinet::addCabinet(location, capacity);
}

bool PickupCabinetController::removeCabinet(int cabinetId) {
    return PickupCabinet::removeCabinet(cabinetId);
}

bool PickupCabinetController::updateCabinetInfo(int cabinetId, const QString& newLocation, int newCapacity) {
    return PickupCabinet::updateCabinetInfo(cabinetId, newLocation, newCapacity);
}

QList<PickupCabinet> PickupCabinetController::getAllCabinets() {
    return PickupCabinet::getAll();
} 