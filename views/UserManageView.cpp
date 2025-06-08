#include "UserManageView.h"
#include "ui_UserManageView.h"
#include "UserController.h"
#include "ExpressCompanyController.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QListWidgetItem>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>

UserManageView::UserManageView(QWidget *parent)
    : QWidget(parent), ui(new Ui::UserManageView)
{
    ui->setupUi(this);
    connect(ui->addUserButton, &QPushButton::clicked, this, &UserManageView::onAddUserClicked);
    connect(ui->modifyUserButton, &QPushButton::clicked, this, &UserManageView::onModifyUserClicked);
    connect(ui->deleteUserButton, &QPushButton::clicked, this, &UserManageView::onDeleteUserClicked);
    connect(ui->resetPasswordButton, &QPushButton::clicked, this, &UserManageView::onResetPasswordClicked);
    connect(ui->userTableWidget, &QTableWidget::cellClicked, this, &UserManageView::onUserTableItemClicked);
    connect(ui->selectExpressCompanyButton, &QPushButton::clicked, this, &UserManageView::onSelectExpressCompanyClicked);
    connect(ui->refreshUserButton, &QPushButton::clicked, this, &UserManageView::refreshUserTable);

    ui->userTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->userTableWidget, &QTableWidget::itemSelectionChanged, this, [this]() {
        auto selectedRanges = ui->userTableWidget->selectedRanges();
        if (!selectedRanges.isEmpty()) {
            int row = selectedRanges.first().topRow();
            onUserTableItemClicked(row, 0);
        }
    });
    refreshUserTable();
}

UserManageView::~UserManageView() {
    delete ui;
}

void UserManageView::refreshUserTable() {
    QStringList headers = {"ID", "用户名", "手机号", "角色", "公司ID"};
    ui->userTableWidget->setColumnCount(headers.size());
    ui->userTableWidget->setHorizontalHeaderLabels(headers);
    ui->userTableWidget->setRowCount(0);
    auto users = UserController::instance().getAllUsers();
    for (int i = 0; i < users.size(); ++i) {
        const User& u = users[i];
        ui->userTableWidget->insertRow(i);
        ui->userTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(u.id)));
        ui->userTableWidget->setItem(i, 1, new QTableWidgetItem(u.username));
        ui->userTableWidget->setItem(i, 2, new QTableWidgetItem(u.phone));
        QString roleStr = (u.role == UserRole::ADMIN) ? "管理员" : (u.role == UserRole::EXPRESSMAN ? "快递员" : "收件人");
        ui->userTableWidget->setItem(i, 3, new QTableWidgetItem(roleStr));
        ui->userTableWidget->setItem(i, 4, new QTableWidgetItem(u.expressCompanyIds));
    }
    ui->userTableWidget->resizeColumnsToContents();
    selectedUserId = -1;
}

void UserManageView::onAddUserClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加用户");
    QFormLayout form(&dialog);
    QLineEdit nameEdit;
    QLineEdit phoneEdit;
    QComboBox roleCombo;
    roleCombo.addItems({"收件人", "快递员", "管理员"});
    form.addRow("用户名:", &nameEdit);
    form.addRow("手机号:", &phoneEdit);
    form.addRow("角色:", &roleCombo);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit.text().trimmed();
        QString phone = phoneEdit.text().trimmed();
        int roleIdx = roleCombo.currentIndex();
        if (name.isEmpty() || phone.isEmpty()) {
            QMessageBox::warning(this, "提示", "用户名和手机号不能为空");
            return;
        }
        if (UserController::instance().addUser(name, phone)) {
            UserController::instance().modifyUserRole(UserController::instance().getAllUsers().last().id, static_cast<UserRole>(roleIdx));
            QMessageBox::information(this, "成功", "添加用户成功，初始密码为123456");
        } else {
            QMessageBox::warning(this, "失败", "添加用户失败，可能已存在");
        }
        refreshUserTable();
    }
}

void UserManageView::onModifyUserClicked() {
    if (selectedUserId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要修改的用户");
        return;
    }
    // 获取当前用户信息
    User user;
    auto users = UserController::instance().getAllUsers();
    for (const auto& u : users) {
        if (u.id == selectedUserId) {
            user = u;
            break;
        }
    }
    QDialog dialog(this);
    dialog.setWindowTitle("修改用户");
    QFormLayout form(&dialog);
    QLineEdit nameEdit;
    nameEdit.setText(user.username);
    QLineEdit phoneEdit;
    phoneEdit.setText(user.phone);
    QComboBox roleCombo;
    roleCombo.addItems({"收件人", "快递员", "管理员"});
    roleCombo.setCurrentIndex(static_cast<int>(user.role));
    form.addRow("用户名:", &nameEdit);
    form.addRow("手机号:", &phoneEdit);
    form.addRow("角色:", &roleCombo);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        QString newName = nameEdit.text().trimmed();
        QString newPhone = phoneEdit.text().trimmed();
        int roleIdx = roleCombo.currentIndex();
        bool ok1 = UserController::instance().modifyUser(selectedUserId, newName, newPhone);
        bool ok2 = UserController::instance().modifyUserRole(selectedUserId, static_cast<UserRole>(roleIdx));
        // 如果角色被改为非快递员，清空公司列
        if (roleIdx != 1) {
            UserController::instance().updateExpressCompanies(selectedUserId, "");
        }
        if (ok1 && ok2) {
            QMessageBox::information(this, "成功", "修改用户信息成功");
        } else {
            QMessageBox::warning(this, "失败", "修改用户信息失败");
        }
        refreshUserTable();
    }
}

void UserManageView::onDeleteUserClicked() {
    if (selectedUserId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要删除的用户");
        return;
    }
    if (UserController::instance().deleteUser(selectedUserId)) {
        QMessageBox::information(this, "成功", "删除用户成功");
    } else {
        QMessageBox::warning(this, "失败", "删除用户失败");
    }
    refreshUserTable();
}

void UserManageView::onResetPasswordClicked() {
    if (selectedUserId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要重置密码的用户");
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle("重置密码");
    QFormLayout form(&dialog);
    QLineEdit passwordEdit;
    passwordEdit.setEchoMode(QLineEdit::Password);
    form.addRow("新密码:", &passwordEdit);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        QString newPwd = passwordEdit.text();
        if (newPwd.isEmpty()) {
            QMessageBox::warning(this, "提示", "新密码不能为空");
            return;
        }
        if (UserController::instance().resetPassword(selectedUserId, newPwd)) {
            QMessageBox::information(this, "成功", "密码已重置");
        } else {
            QMessageBox::warning(this, "失败", "重置密码失败");
        }
    }
}

void UserManageView::onUserTableItemClicked(int row, int) {
    QTableWidgetItem* idItem = ui->userTableWidget->item(row, 0);
    QTableWidgetItem* nameItem = ui->userTableWidget->item(row, 1);
    QTableWidgetItem* phoneItem = ui->userTableWidget->item(row, 2);
    QTableWidgetItem* roleItem = ui->userTableWidget->item(row, 3);
    QTableWidgetItem* companyItem = ui->userTableWidget->item(row, 4);
    if (!idItem) return;
    selectedUserId = idItem->text().toInt();
    ui->userNameEdit->setText(nameItem ? nameItem->text() : "");
    ui->userPhoneEdit->setText(phoneItem ? phoneItem->text() : "");
    if (roleItem) {
        if (roleItem->text() == "收件人") ui->userRoleCombo->setCurrentIndex(0);
        else if (roleItem->text() == "快递员") ui->userRoleCombo->setCurrentIndex(1);
        else ui->userRoleCombo->setCurrentIndex(2);
    }
    // 控制按钮和公司显示
    if (roleItem && roleItem->text() == "快递员") {
        ui->selectExpressCompanyButton->setEnabled(true);
        ui->expressCompanyLineEdit->setText(companyItem ? companyItem->text() : "");
    } else {
        ui->selectExpressCompanyButton->setEnabled(false);
        ui->expressCompanyLineEdit->clear();
    }
}

void UserManageView::onSelectExpressCompanyClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("选择快递公司");
    QVBoxLayout layout(&dialog);
    QTableView tableView(&dialog);
    QStandardItemModel model;
    tableView.setModel(&model);
    tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView.setSelectionMode(QAbstractItemView::MultiSelection);
    tableView.horizontalHeader()->setStretchLastSection(true);
    // 填充公司数据
    auto companies = ExpressCompanyController::instance().getAllCompanies();
    model.setHorizontalHeaderLabels({"公司ID", "公司名称"});
    for (int i = 0; i < companies.size(); ++i) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(companies[i].companyId));
        row << new QStandardItem(companies[i].name);
        model.appendRow(row);
    }
    layout.addWidget(&tableView);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout.addWidget(&buttonBox);
    // 自动选中当前快递员已隶属的公司
    QStringList currentIds;
    if (selectedUserId != -1) {
        auto users = UserController::instance().getAllUsers();
        for (const auto& u : users) {
            if (u.id == selectedUserId) {
                currentIds = u.expressCompanyIds.split(",", Qt::SkipEmptyParts);
                break;
            }
        }
    }
    for (int i = 0; i < companies.size(); ++i) {
        if (currentIds.contains(QString::number(companies[i].companyId))) {
            tableView.selectRow(i);
        }
    }
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, [&](){
        QModelIndexList selected = tableView.selectionModel()->selectedRows();
        QStringList names;
        QStringList ids;
        for (const QModelIndex& idx : selected) {
            int row = idx.row();
            names << companies[row].name;
            ids << QString::number(companies[row].companyId);
        }
        ui->expressCompanyLineEdit->setText(names.join(","));
        selectedCompanyNames = names;
        selectedCompanyIdsStr = ids.join(",");
        // 更新数据库
        bool ok = UserController::instance().updateExpressCompanies(selectedUserId, selectedCompanyIdsStr);
        if (ok) {
            QMessageBox::information(this, "成功", "快递员公司已更新");
        } else {
            QMessageBox::warning(this, "失败", "更新失败");
        }
        refreshUserTable();
        dialog.accept();
    });
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
} 