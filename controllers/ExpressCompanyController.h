#pragma once
#include <QObject>
#include "ExpressCompany.h"

class ExpressCompanyController : public QObject {
    Q_OBJECT
public:
    static ExpressCompanyController& instance() {
        static ExpressCompanyController instance;
        return instance;
    }
    static bool createTable();
    bool addCompany(const QString& name, double fee);
    bool removeCompany(int companyId);
    bool updateFee(int companyId, double newFee);
    QList<ExpressCompany> getAllCompanies();
}; 