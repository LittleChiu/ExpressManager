#include "UserController.h"
#include <QSqlQuery>
#include <QVariant>
#include <QCryptographicHash>
#include <optional>

QString encryptPassword(const QString& password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool UserController::addUser(const QString& name, const QString& phone, const QString& password) {
    if (!getUserByUserName(name).has_value() || !getUserByPhone(phone).has_value()) return false;
    QSqlQuery query;
    query.prepare("INSERT INTO User (username, phone, passwordEncrypted, role, expressCompanyIds) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(phone);
    query.addBindValue(encryptPassword(password));
    query.addBindValue(static_cast<int>(UserRole::RECIPIENT));
    query.addBindValue("");
    return query.exec();
}

bool UserController::deleteUser(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM User WHERE id = ?");
    query.addBindValue(userId);
    return query.exec();
}

bool UserController::modifyUser(int userId, const QString& newName, const QString& newPhone) {
    if (!getUserByPhone(newPhone).has_value() || !getUserByUserName(newName).has_value()) return false;
    QSqlQuery query;
    query.prepare("UPDATE User SET username = ?, phone = ? WHERE id = ?");
    query.addBindValue(newName);
    query.addBindValue(newPhone);
    query.addBindValue(userId);
    return query.exec();
}

bool UserController::modifyUserRole(int userId, UserRole newRole) {
    QSqlQuery query;
    query.prepare("UPDATE User SET role = ? WHERE id = ?");
    query.addBindValue(static_cast<int>(newRole));
    query.addBindValue(userId);
    return query.exec();
}

bool UserController::resetPassword(int userId, const QString& newPassword) {
    QSqlQuery query;
    query.prepare("UPDATE User SET passwordEncrypted = ? WHERE id = ?");
    query.addBindValue(encryptPassword(newPassword));
    query.addBindValue(userId);
    return query.exec();
}

bool UserController::updateExpressCompanies(int userId, const QString& expressCompanyIds) {
    QSqlQuery query;
    query.prepare("UPDATE User SET expressCompanyIds = ? WHERE id = ?");
    query.addBindValue(expressCompanyIds);
    query.addBindValue(userId);
    return query.exec();
}

QList<User> UserController::getAllUsers() {
    QList<User> list;
    QSqlQuery query("SELECT * FROM User");
    while (query.next()) {
        list.append(User::fromQuery(query));
    }
    return list;
}

std::optional<User> UserController::loginByPhone(const QString& phone, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM User WHERE phone = ?");
    query.addBindValue(phone);
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    QString encrypted = query.value("passwordEncrypted").toString();
    if (encrypted == encryptPassword(password)) {
        return User::fromQuery(query);
    }
    return std::nullopt;
}

std::optional<User> UserController::loginByUserName(const QString& name, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM User WHERE username = ?");
    query.addBindValue(name);
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    QString encrypted = query.value("passwordEncrypted").toString();
    if (encrypted == encryptPassword(password)) {
        return User::fromQuery(query);
    }
    return std::nullopt;
}

std::optional<User> UserController::getUserById(int userId) {
    auto users = getAllUsers();
    for (const auto& u : users) {
        if (u.id == userId) return u;
    }
    return std::nullopt;
}
std::optional<User> UserController::getUserByUserName(const QString& userName) {
    QSqlQuery query;
    query.prepare("SELECT * FROM User WHERE username = ?");
    query.addBindValue(userName);

    if (query.exec() && query.next()) {
        return User::fromQuery(query);
    }
    return std::nullopt;
}

std::optional<User> UserController::getUserByPhone(const QString& phone) {
    QSqlQuery query;
    query.prepare("SELECT * FROM User WHERE phone = ?");
    query.addBindValue(phone);

    if (query.exec() && query.next()) {
        return User::fromQuery(query);
    }
    return std::nullopt;
}

bool UserController::createTable() {
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