#pragma once
#include <QString>
#include <QSqlQuery>

class ExpressCompany {
public:
    int companyId;
    QString name;
    double packageFee;

    static ExpressCompany fromQuery(const QSqlQuery &query) {
        ExpressCompany c;
        c.companyId = query.value("companyId").toInt();
        c.name = query.value("name").toString();
        c.packageFee = query.value("packageFee").toDouble();
        return c;
    }
};
