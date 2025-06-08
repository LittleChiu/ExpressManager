#pragma once
#include <QObject>
#include "User.h"
#include <optional>

class UserController : public QObject {
    Q_OBJECT
public:
    static UserController& instance() {
        static UserController instance;
        return instance;
    }
    UserController(const UserController&) = delete;
    void operator=(const UserController&) = delete;

    bool addUser(const QString& name, const QString& phone, const QString& password = "123456");
    bool deleteUser(int userId);
    bool modifyUser(int userId, const QString& newName, const QString& newPhone);
    bool modifyUserRole(int userId, UserRole newRole);
    bool resetPassword(int userId, const QString& newPassword);
    std::optional<User> loginByPhone(const QString& phone, const QString& password);
    QList<User> getAllUsers();
    User getUserById(int userId);
    bool updateExpressCompanies(int userId, const QString& expressCompanyIds);
    // 其他接口...
private:
    UserController() {}
}; 