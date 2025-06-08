#include "ExpressCompanyController.h"

bool ExpressCompanyController::addCompany(const QString& name, double fee) {
    return ExpressCompany::addCompany(name, fee);
}

bool ExpressCompanyController::removeCompany(int companyId) {
    return ExpressCompany::removeCompany(companyId);
}

bool ExpressCompanyController::updateFee(int companyId, double newFee) {
    return ExpressCompany::updateFee(companyId, newFee);
}

QList<ExpressCompany> ExpressCompanyController::getAllCompanies() {
    return ExpressCompany::getAll();
} 