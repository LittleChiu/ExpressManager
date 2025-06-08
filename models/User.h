#pragma once
#include <QString>
#include <QSqlQuery>
#include <QList>
#include "enums.h"

class User {
public:
    int id;
    QString username;
    QString passwordEncrypted;
    QString phone;
    UserRole role;
    QString expressCompanyIds; // 快递员隶属公司ID列表，逗号分隔

    static User fromQuery(const QSqlQuery& query) {
        User u;
        u.id = query.value("id").toInt();
        u.username = query.value("username").toString();
        u.phone = query.value("phone").toString();
        u.role = static_cast<UserRole>(query.value("role").toInt());
        u.expressCompanyIds = query.value("expressCompanyIds").toString();
        return u;
    }

}; 