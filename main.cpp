#include <QApplication>
#include "db/DatabaseManager.h"
#include "views/LoginView.h"
#include "views/AdminView.h"
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DatabaseManager::initDatabase();
    LoginView login;
    login.show();
    QObject::connect(&login, &LoginView::loginSuccess, [&](const User& user){
        login.hide();
        QWidget* next = nullptr;
        switch (user.role) {
        case UserRole::ADMIN:
            next = new AdminView(user.id);
            break;
        case UserRole::EXPRESSMAN:
            // TODO: 快递员界面
            break;
        case UserRole::RECIPIENT:
            // TODO: 收件人界面
            break;
        }
        if (next) {
            next->setAttribute(Qt::WA_DeleteOnClose);
            next->show();
        }
    });
    return a.exec();
}