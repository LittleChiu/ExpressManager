#include <QApplication>
#include "db/DatabaseManager.h"
#include "views/LoginView.h"
#include "views/AdminView.h"
#include "views/ExpressmanView.h"
#include "views/RecipientView.h"
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DatabaseManager::initDatabase();
    LoginView login;
    login.show();
    QObject::connect(&login, &LoginView::loginSuccess, [&](const User &user) {
        login.hide();
        QWidget *next = nullptr;
        switch (user.role) {
            case UserRole::ADMIN:
                next = new AdminView(user.id);
                break;
            case UserRole::EXPRESSMAN:
                next = new ExpressmanView(user.id);
                break;
            case UserRole::RECIPIENT:
                next = new RecipientView(user.id);
                break;
        }
        if (next) {
            next->setAttribute(Qt::WA_DeleteOnClose);
            next->show();
            QObject::connect(next, SIGNAL(logoutRequested()), &a, SLOT(quit()));
            QObject::connect(next, SIGNAL(switchAccountRequested()), next, SLOT(close()));
            QObject::connect(next, SIGNAL(switchAccountRequested()), &login, SLOT(show()));
        }
    });


    return a.exec();
}
