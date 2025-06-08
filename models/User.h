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

    static bool createTable();
    static User fromQuery(const QSqlQuery& query);
    static QList<User> getAll();
    static bool addUser(const QString& username, const QString& phone, const QString& password, UserRole role, const QString& expressCompanyIds = "");
}; 