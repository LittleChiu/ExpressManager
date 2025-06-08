#include "UserController.h"
#include <QSqlQuery>
#include <QVariant>
#include <QCryptographicHash>
#include <optional>

namespace {
QString encryptPassword(const QString& password) {
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}
}

bool UserController::addUser(const QString& name, const QString& phone, const QString& password) {
    return User::addUser(name,phone,encryptPassword(password),UserRole::RECIPIENT);
}

bool UserController::deleteUser(int userId) {
    QSqlQuery query;
    query.prepare("DELETE FROM User WHERE id = ?");
    query.addBindValue(userId);
    return query.exec();
}

bool UserController::modifyUser(int userId, const QString& newName, const QString& newPhone) {
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
    return User::getAll();
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

User UserController::getUserById(int userId) {
    auto users = getAllUsers();
    for (const auto& u : users) {
        if (u.id == userId) return u;
    }
    return User();
} 