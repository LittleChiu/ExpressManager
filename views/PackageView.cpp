#include "PackageView.h"
#include "ui_PackageView.h"
#include "PackageController.h"
#include "ExpressCompanyController.h"
#include "UserController.h"
#include "PickupCabinetController.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDateTime>
#include <random>

QString PackageView::generatePickupCode() {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 35);
    QString code;
    for (int i = 0; i < 6; ++i) {
        code += chars[dis(gen)];
    }
    return code;
}

PackageView::PackageView(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::PackageView), currentUserId(userId)
{
    ui->setupUi(this);
    connect(ui->addPackageButton, &QPushButton::clicked, this, &PackageView::onAddPackageClicked);
    connect(ui->updateStatusButton, &QPushButton::clicked, this, &PackageView::onUpdateStatusClicked);
    connect(ui->packageTableWidget, &QTableWidget::cellClicked, this, &PackageView::onPackageTableItemClicked);
    connect(ui->refreshPackageButton, &QPushButton::clicked, this, &PackageView::refreshPackageTable);
    refreshPackageTable();
}

PackageView::~PackageView() {
    delete ui;
}

void PackageView::refreshPackageTable() {
    QStringList headers = {"ID", "体积", "易碎", "状态", "入柜时间", "出柜时间", "取件码","位置", "公司ID", "收件人ID", "快递员ID"};
    ui->packageTableWidget->setColumnCount(headers.size());
    ui->packageTableWidget->setHorizontalHeaderLabels(headers);
    ui->packageTableWidget->setRowCount(0);
    auto pkgs = PackageController::instance().getAllPackages();
    // 判断当前用户是否为管理员
    bool isAdmin = false;
    if (currentUserId != -1) {
        User user = UserController::instance().getUserById(currentUserId);
        isAdmin = (user.role == UserRole::ADMIN);
    }
    int rowIdx = 0;
    auto users = UserController::instance().getAllUsers();
    for (int i = 0; i < pkgs.size(); ++i) {
        const Package& p = pkgs[i];
        if (!isAdmin && p.expressmanId != currentUserId) continue; // 非管理员只能看自己投放的
        ui->packageTableWidget->insertRow(rowIdx);
        ui->packageTableWidget->setItem(rowIdx, 0, new QTableWidgetItem(QString::number(p.packageId)));
        ui->packageTableWidget->setItem(rowIdx, 1, new QTableWidgetItem(QString::number(p.volume)));
        ui->packageTableWidget->setItem(rowIdx, 2, new QTableWidgetItem(p.isFragile ? "是" : "否"));
        QString statusStr = (p.status == PackageStatus::STORED) ? "已存入柜" : (p.status == PackageStatus::PICKUPED ? "已取件" : "异常");
        ui->packageTableWidget->setItem(rowIdx, 3, new QTableWidgetItem(statusStr));
        // 入柜时间
        QString storedTimeStr = QDateTime::fromSecsSinceEpoch(p.storedTime).toString("yyyy-MM-dd HH:mm:ss");
        ui->packageTableWidget->setItem(rowIdx, 4, new QTableWidgetItem(storedTimeStr));
        // 出柜时间
        QString pickupTimeStr = (p.pickupTime == -1) ? "-1" : QDateTime::fromSecsSinceEpoch(p.pickupTime).toString("yyyy-MM-dd HH:mm:ss");
        ui->packageTableWidget->setItem(rowIdx, 5, new QTableWidgetItem(pickupTimeStr));
        ui->packageTableWidget->setItem(rowIdx, 6, new QTableWidgetItem(p.pickupCode));
        ui->packageTableWidget->setItem(rowIdx, 7, new QTableWidgetItem(p.location));
        ui->packageTableWidget->setItem(rowIdx, 8, new QTableWidgetItem(QString::number(p.expressCompanyId)));
        // 收件人用户名(ID)
        User recipient = UserController::instance().getUserById(p.recipientId);
        QString recipientStr = recipient.username.isEmpty() ? "-" : QString("%1(%2)").arg(recipient.username).arg(recipient.id);
        ui->packageTableWidget->setItem(rowIdx, 9, new QTableWidgetItem(recipientStr));
        // 快递员用户名(ID)
        User expressman = UserController::instance().getUserById(p.expressmanId);
        QString expressmanStr = expressman.username.isEmpty() ? "-" : QString("%1(%2)").arg(expressman.username).arg(expressman.id);
        ui->packageTableWidget->setItem(rowIdx, 10, new QTableWidgetItem(expressmanStr));
        rowIdx++;
    }
    ui->packageTableWidget->resizeColumnsToContents();
    selectedPackageId = -1;
}

void PackageView::onAddPackageClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加包裹");
    QFormLayout form(&dialog);
    QDoubleSpinBox volumeSpin;
    QComboBox fragileCombo; fragileCombo.addItems({"否", "是"});
    QComboBox statusCombo; statusCombo.addItems({"已存入柜", "已取件", "异常"});
    QComboBox locationCombo;
    QComboBox companyCombo, recipientCombo;
    recipientCombo.setEditable(true); // 允许自定义输入
    // 填充柜子
    auto cabinets = PickupCabinetController::instance().getAllCabinets();
    for (const auto& cab : cabinets) locationCombo.addItem(QString("%1(%2)").arg(cab.location).arg(cab.cabinetId), cab.cabinetId);
    // 填充公司和用户
    auto companies = ExpressCompanyController::instance().getAllCompanies();
    QStringList allowedIds;
    bool isExpressman = false;
    if (currentUserId != -1) {
        User user = UserController::instance().getUserById(currentUserId);
        isExpressman = (user.role == UserRole::EXPRESSMAN);
        if (isExpressman) {
            allowedIds = user.expressCompanyIds.split(",", Qt::SkipEmptyParts);
        }
    }
    for (const auto& c : companies) {
        if (!isExpressman || allowedIds.contains(QString::number(c.companyId))) {
            companyCombo.addItem(QString("%1(%2)").arg(c.name).arg(c.companyId), c.companyId);
        }
    }
    auto users = UserController::instance().getAllUsers();
    for (const auto& u : users) recipientCombo.addItem(QString("%1(%2)").arg(u.username).arg(u.id), u.id);
    form.addRow("体积:", &volumeSpin);
    form.addRow("易碎:", &fragileCombo);
    form.addRow("状态:", &statusCombo);
    form.addRow("位置:", &locationCombo);
    form.addRow("快递公司:", &companyCombo);
    form.addRow("收件人:", &recipientCombo);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        int cabIdx = locationCombo.currentIndex();
        if (cabIdx < 0) {
            QMessageBox::warning(this, "提示", "请选择快递柜");
            return;
        }
        const PickupCabinet& cab = cabinets[cabIdx];
        // 通过PackageController查询该柜子当前存放包裹列表
        auto storedPkgs = PackageController::instance().queryByLocationAndStatus(cab.location, PackageStatus::STORED);
        double storedVer = 0;
        for (Package & pag:storedPkgs) {
            storedVer+=pag.volume;
            qDebug() << storedVer <<" " << pag.volume;
        }
        qDebug() << cab.capacity;
        if (storedVer >= cab.capacity) {
            QMessageBox::warning(this, "提示", QString("柜子[%1]已满").arg(cab.location));
            return;
        } else if (storedVer + volumeSpin.value() > cab.capacity) {
            QMessageBox::warning(this, "提示", QString("柜子[%1]放不下这个包裹").arg(cab.location));
            return;
        }
        Package pkg;
        pkg.volume = volumeSpin.value();
        pkg.isFragile = fragileCombo.currentIndex() == 1;
        pkg.status = static_cast<PackageStatus>(statusCombo.currentIndex());
        pkg.location = cab.location;
        pkg.storedTime = QDateTime::currentSecsSinceEpoch();
        pkg.expressCompanyId = companyCombo.currentData().toInt();
        QString recipientInput = recipientCombo.currentText().trimmed();
        int recipientId = -1;
        QString recipientPhone;
        // 检查输入是否为已存在的用户
        for (const auto& u : users) {
            QString label = QString("%1(%2)").arg(u.username).arg(u.id);
            if (recipientInput == label || recipientInput == u.username || recipientInput == u.phone) {
                recipientId = u.id;
                recipientPhone = u.phone;
                break;
            }
        }
        if (recipientId == -1) {
            // 尝试将输入当作手机号
            QString phone = recipientInput;
            if (phone.length() < 6) {
                QMessageBox::warning(this, "提示", "自定义收件人请输入手机号");
                return;
            }
            QString username = phone;
            QString password = phone.right(6);
            if (!UserController::instance().addUser(username, phone, password)) {
                QMessageBox::warning(this, "失败", "自动创建收件人失败");
                return;
            }
            // 刷新用户列表，获取新用户id
            users = UserController::instance().getAllUsers();
            for (const auto& u : users) {
                if (u.phone == phone) {
                    recipientId = u.id;
                    recipientPhone = u.phone;
                    break;
                }
            }
            QMessageBox::information(this, "提示", QString("已自动创建新收件人，手机号：%1，初始密码：%2").arg(phone).arg(password));
        }
        pkg.recipientId = recipientId;
        pkg.expressmanId = currentUserId;
        pkg.pickupCode = generatePickupCode();
        if (PackageController::instance().addPackage(pkg)) {
            QMessageBox::information(this, "成功", "添加包裹成功\n取件码: " + pkg.pickupCode);
        } else {
            QMessageBox::warning(this, "失败", "添加包裹失败");
        }
        refreshPackageTable();
    }
}

void PackageView::onUpdateStatusClicked() {
    if (selectedPackageId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要修改状态的包裹");
        return;
    }
    QDialog dialog(this);
    dialog.setWindowTitle("修改包裹状态");
    QFormLayout form(&dialog);
    QComboBox statusCombo; statusCombo.addItems({"已存入柜", "已取件", "异常"});
    form.addRow("新状态:", &statusCombo);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        PackageStatus newStatus = static_cast<PackageStatus>(statusCombo.currentIndex());
        if (PackageController::instance().updateStatus(selectedPackageId, newStatus)) {
            QMessageBox::information(this, "成功", "修改状态成功");
        } else {
            QMessageBox::warning(this, "失败", "修改状态失败");
        }
        refreshPackageTable();
    }
}

void PackageView::onPackageTableItemClicked(int row, int) {
    QTableWidgetItem* idItem = ui->packageTableWidget->item(row, 0);
    if (!idItem) return;
    selectedPackageId = idItem->text().toInt();
} 