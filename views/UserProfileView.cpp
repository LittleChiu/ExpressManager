#include "UserProfileView.h"
#include "ui_UserProfileView.h"
#include "UserController.h"
#include <QMessageBox>
#include <QInputDialog>

UserProfileView::UserProfileView(QWidget *parent)
    : QWidget(parent), ui(new Ui::UserProfileView)
{
    ui->setupUi(this);
    connect(ui->updatePhoneButton, &QPushButton::clicked, this, &UserProfileView::onUpdatePhoneClicked);
    connect(ui->updatePasswordButton, &QPushButton::clicked, this, &UserProfileView::onUpdatePasswordClicked);
}

UserProfileView::~UserProfileView() {
    delete ui;
}

void UserProfileView::setUserId(int id) {
    userId = id;
    User user = UserController::instance().getUserById(userId);
    ui->phoneLabel->setText(user.phone);
}

void UserProfileView::onUpdatePhoneClicked() {
    if (userId == -1) return;
    QString newPhone = QInputDialog::getText(this, "修改手机号", "新手机号:");
    if (newPhone.isEmpty()) return;
    auto user = UserController::instance().getUserById(userId);
    if (UserController::instance().modifyUser(userId, user.username, newPhone)) {
        QMessageBox::information(this, "成功", "手机号修改成功");
        ui->phoneLabel->setText(newPhone);
    } else {
        QMessageBox::warning(this, "失败", "手机号修改失败");
    }
}

void UserProfileView::onUpdatePasswordClicked() {
    if (userId == -1) return;
    QString newPwd = QInputDialog::getText(this, "修改密码", "新密码:", QLineEdit::Password);
    if (newPwd.isEmpty()) return;
    if (UserController::instance().resetPassword(userId, newPwd)) {
        QMessageBox::information(this, "成功", "密码修改成功");
    } else {
        QMessageBox::warning(this, "失败", "密码修改失败");
    }
} 