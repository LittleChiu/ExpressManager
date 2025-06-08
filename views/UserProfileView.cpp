#include "UserProfileView.h"
#include "ui_UserProfileView.h"
#include "UserController.h"
#include "ExpressCompanyController.h"
#include <QMessageBox>
#include <QInputDialog>

UserProfileView::UserProfileView(QWidget *parent)
    : QWidget(parent), ui(new Ui::UserProfileView)
{
    ui->setupUi(this);
    connect(ui->updatePhoneButton, &QPushButton::clicked, this, &UserProfileView::onUpdatePhoneClicked);
    connect(ui->updatePasswordButton, &QPushButton::clicked, this, &UserProfileView::onUpdatePasswordClicked);
    connect(ui->updateUsernameButton, &QPushButton::clicked, this, &UserProfileView::onUpdateUsernameClicked);
}

UserProfileView::~UserProfileView() {
    delete ui;
}

void UserProfileView::setUserId(int id) {
    userId = id;
    User user = UserController::instance().getUserById(userId);
    ui->usernameLabel->setText(user.username);
    ui->phoneLabel->setText(user.phone);

    bool show = (user.role == UserRole::EXPRESSMAN);
    ui->labelCompanyTitle->setVisible(show);
    ui->companyLabel->setVisible(show);
    refreshCompanyLabel();
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

void UserProfileView::onUpdateUsernameClicked() {
    if (userId == -1) return;
    QString newName = QInputDialog::getText(this, "修改用户名", "新用户名:");
    if (newName.isEmpty()) return;
    auto user = UserController::instance().getUserById(userId);
    if (UserController::instance().modifyUser(userId, newName, user.phone)) {
        QMessageBox::information(this, "成功", "用户名修改成功");
        ui->usernameLabel->setText(newName);
    } else {
        QMessageBox::warning(this, "失败", "用户名修改失败");
    }
}

void UserProfileView::refreshCompanyLabel() {
    if (userId == -1) {
        ui->companyLabel->setText("-");
        return;
    }
    User user = UserController::instance().getUserById(userId);
    if (user.role != UserRole::EXPRESSMAN) {
        ui->companyLabel->setText("-");
        return;
    }
    QStringList idList = user.expressCompanyIds.split(",", Qt::SkipEmptyParts);
    if (idList.isEmpty()) {
        ui->companyLabel->setText("无");
        return;
    }
    auto companies = ExpressCompanyController::instance().getAllCompanies();
    QStringList names;
    for (const QString& id : idList) {
        for (const auto& c : companies) {
            if (QString::number(c.companyId) == id) {
                names << c.name;
                break;
            }
        }
    }
    ui->companyLabel->setText(names.isEmpty() ? "无" : names.join(", "));
} 