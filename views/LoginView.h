#pragma once
#include <QWidget>
#include "User.h"

namespace Ui {
class LoginView;
}

class LoginView : public QWidget {
    Q_OBJECT
public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

signals:
    void loginSuccess(const User& user);

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Ui::LoginView *ui;
}; 