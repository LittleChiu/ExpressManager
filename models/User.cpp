#include "User.h"
#include <QSqlQuery>
#include <QVariant>
#include <QList>

bool User::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS User ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "username TEXT,"
                      "phone TEXT,"
                      "passwordEncrypted TEXT,"
                      "role INTEGER,"
                      "expressCompanyIds TEXT DEFAULT ''"
                      ")");
}

User User::fromQuery(const QSqlQuery& query) {
    User u;
    u.id = query.value("id").toInt();
    u.username = query.value("username").toString();
    u.phone = query.value("phone").toString();
    u.role = static_cast<UserRole>(query.value("role").toInt());
    u.expressCompanyIds = query.value("expressCompanyIds").toString();
    return u;
}

QList<User> User::getAll() {
    QList<User> list;
    QSqlQuery query("SELECT * FROM User");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

bool User::addUser(const QString& username, const QString& phone, const QString& password, UserRole role, const QString& expressCompanyIds) {
    QSqlQuery query;
    query.prepare("INSERT INTO User (username, phone, passwordEncrypted, role, expressCompanyIds) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(phone);
    query.addBindValue(password);
    query.addBindValue(static_cast<int>(role));
    query.addBindValue(expressCompanyIds);
    return query.exec();
} 