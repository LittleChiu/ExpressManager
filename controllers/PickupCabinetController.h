#pragma once
#include <QObject>
#include "PickupCabinet.h"

class PickupCabinetController : public QObject {
    Q_OBJECT

public:
    static PickupCabinetController &instance() {
        static PickupCabinetController instance;
        return instance;
    }

    bool addCabinet(const QString &location, int capacity);

    bool removeCabinet(int cabinetId);

    bool updateCabinetInfo(int cabinetId, const QString &newLocation, int newCapacity);

    QList<PickupCabinet> getAllCabinets();

    bool createTable();
};
