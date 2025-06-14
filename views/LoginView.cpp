#include "LoginView.h"
#include "ui_LoginView.h"
#include <QPushButton>
#include <QLineEdit>
#include "UserController.h"
#include <QMessageBox>
#include "QuickPickupView.h"

LoginView::LoginView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginView)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginView::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &LoginView::onRegisterClicked);

    // 在tabPickup中添加QuickPickupView
    quickPickupView = new QuickPickupView(this);
    QVBoxLayout* layout = new QVBoxLayout(ui->tabPickup);
    layout->addWidget(quickPickupView);
    ui->tabPickup->setLayout(layout);
}

LoginView::~LoginView() {
    delete ui;
}

void LoginView::onLoginClicked() {
    QString phone = ui->phoneEditLogin->text();
    QString password = ui->passwordEditLogin->text();
    auto userOpt = UserController::instance().loginByPhone(phone, password);
    if (!userOpt.has_value()) {
        auto userOpt2 = UserController::instance().loginByUserName(phone, password);
        if (userOpt2.has_value()) {
            emit loginSuccess(userOpt2.value());
            QMessageBox::information(this, "登录成功", "欢迎回来！");
        }else {
            QMessageBox::warning(this, "登录失败", "用户名手机号或密码错误");
        }

    } else {
        emit loginSuccess(userOpt.value());
        QMessageBox::information(this, "登录成功", "欢迎回来！");
    }
}

void LoginView::onRegisterClicked() {
    QString username = ui->usernameEditRegister->text();
    QString phone = ui->phoneEditRegister->text();
    QString password = ui->passwordEditRegister->text();
    if (phone.length() != 11) {
        QMessageBox::warning(this, "注册失败", "手机号不合法.");
        return;
    }
    bool ok = UserController::instance().addUser(username, phone, password);
    if (!ok) {
        QMessageBox::warning(this, "注册失败", "注册信息有误或用户已存在.");
    } else {
        QMessageBox::information(this, "注册成功", "注册成功! 请登录.");
        ui->tabWidget->setCurrentIndex(1);
    }
}