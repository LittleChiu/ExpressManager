#pragma once
#include <QString>
#include <QSqlQuery>

class ExpressCompany {
public:
    int companyId;
    QString name;
    double packageFee;

    static bool createTable();
    static ExpressCompany fromQuery(const QSqlQuery& query);
    static QList<ExpressCompany> getAll();
    static bool addCompany(const QString& name, double fee);
    static bool removeCompany(int companyId);
    static bool updateFee(int companyId, double newFee);
}; 