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

PackageView::PackageView(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::PackageView), currentUserId(userId)
{
    ui->setupUi(this);
    connect(ui->addPackageButton, &QPushButton::clicked, this, &PackageView::onAddPackageClicked);
    connect(ui->updateStatusButton, &QPushButton::clicked, this, &PackageView::onUpdateStatusClicked);
    connect(ui->packageTableWidget, &QTableWidget::cellClicked, this, &PackageView::onPackageTableItemClicked);
    refreshPackageTable();
}

PackageView::~PackageView() {
    delete ui;
}

void PackageView::refreshPackageTable() {
    QStringList headers = {"ID", "体积", "重量", "易碎", "状态", "位置", "公司ID", "收件人ID", "快递员ID"};
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
    for (int i = 0; i < pkgs.size(); ++i) {
        const Package& p = pkgs[i];
        if (!isAdmin && p.expressmanId != currentUserId) continue; // 非管理员只能看自己投放的
        ui->packageTableWidget->insertRow(rowIdx);
        ui->packageTableWidget->setItem(rowIdx, 0, new QTableWidgetItem(QString::number(p.packageId)));
        ui->packageTableWidget->setItem(rowIdx, 1, new QTableWidgetItem(QString::number(p.volume)));
        ui->packageTableWidget->setItem(rowIdx, 2, new QTableWidgetItem(QString::number(p.weight)));
        ui->packageTableWidget->setItem(rowIdx, 3, new QTableWidgetItem(p.isFragile ? "是" : "否"));
        QString statusStr = (p.status == PackageStatus::STORED) ? "已存入柜" : (p.status == PackageStatus::PICKUPED ? "已取件" : "异常");
        ui->packageTableWidget->setItem(rowIdx, 4, new QTableWidgetItem(statusStr));
        ui->packageTableWidget->setItem(rowIdx, 5, new QTableWidgetItem(p.location));
        ui->packageTableWidget->setItem(rowIdx, 6, new QTableWidgetItem(QString::number(p.expressCompanyId)));
        ui->packageTableWidget->setItem(rowIdx, 7, new QTableWidgetItem(QString::number(p.recipientId)));
        ui->packageTableWidget->setItem(rowIdx, 8, new QTableWidgetItem(QString::number(p.expressmanId)));
        rowIdx++;
    }
    ui->packageTableWidget->resizeColumnsToContents();
    selectedPackageId = -1;
}

void PackageView::onAddPackageClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加包裹");
    QFormLayout form(&dialog);
    QDoubleSpinBox volumeSpin, weightSpin;
    volumeSpin.setMinimum(0.01); volumeSpin.setMaximum(9999.99); volumeSpin.setDecimals(2);
    weightSpin.setMinimum(0.01); weightSpin.setMaximum(9999.99); weightSpin.setDecimals(2);
    QComboBox fragileCombo; fragileCombo.addItems({"否", "是"});
    QComboBox statusCombo; statusCombo.addItems({"已存入柜", "已取件", "异常"});
    QComboBox locationCombo;
    QComboBox companyCombo, recipientCombo;
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
    form.addRow("重量:", &weightSpin);
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
        // 通过PackageController查询该柜子当前存放包裹列表（SQL实现）
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
        } else if (storedVer + weightSpin.value() > cab.capacity) {
            QMessageBox::warning(this, "提示", QString("柜子[%1]放不下这个包裹").arg(cab.location));
            return;
        }
        Package pkg;
        pkg.volume = volumeSpin.value();
        pkg.weight = weightSpin.value();
        pkg.isFragile = fragileCombo.currentIndex() == 1;
        pkg.status = static_cast<PackageStatus>(statusCombo.currentIndex());
        pkg.location = cab.location;
        pkg.storedTime = QDateTime::currentSecsSinceEpoch();
        pkg.expressCompanyId = companyCombo.currentData().toInt();
        pkg.recipientId = recipientCombo.currentData().toInt();
        pkg.expressmanId = currentUserId;
        if (PackageController::instance().addPackage(pkg)) {
            QMessageBox::information(this, "成功", "添加包裹成功");
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