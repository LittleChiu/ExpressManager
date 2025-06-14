#include "PickupCabinetView.h"
#include "ui_PickupCabinetView.h"
#include "PickupCabinetController.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "PackageController.h"

PickupCabinetView::PickupCabinetView(QWidget *parent)
    : QWidget(parent), ui(new Ui::PickupCabinetView) {
    ui->setupUi(this);
    connect(ui->addCabinetButton, &QPushButton::clicked, this, &PickupCabinetView::onAddCabinetClicked);
    connect(ui->removeCabinetButton, &QPushButton::clicked, this, &PickupCabinetView::onRemoveCabinetClicked);
    connect(ui->updateCabinetButton, &QPushButton::clicked, this, &PickupCabinetView::onUpdateCabinetClicked);
    connect(ui->cabinetTableWidget, &QTableWidget::cellClicked, this, &PickupCabinetView::onCabinetTableItemClicked);
    connect(ui->viewPackagesButton, &QPushButton::clicked, this, &PickupCabinetView::onViewPackagesClicked);
    connect(ui->refreshCabinetButton, &QPushButton::clicked, this, &PickupCabinetView::refreshCabinetTable);
    refreshCabinetTable();
}

PickupCabinetView::~PickupCabinetView() {
    delete ui;
}

void PickupCabinetView::refreshCabinetTable() {
    QStringList headers = {"ID", "位置", "容量"};
    ui->cabinetTableWidget->setColumnCount(headers.size());
    ui->cabinetTableWidget->setHorizontalHeaderLabels(headers);
    ui->cabinetTableWidget->setRowCount(0);
    auto cabinets = PickupCabinetController::instance().getAllCabinets();
    for (int i = 0; i < cabinets.size(); ++i) {
        const PickupCabinet &c = cabinets[i];
        ui->cabinetTableWidget->insertRow(i);
        ui->cabinetTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(c.cabinetId)));
        ui->cabinetTableWidget->setItem(i, 1, new QTableWidgetItem(c.location));
        ui->cabinetTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(c.capacity)));
    }
    ui->cabinetTableWidget->resizeColumnsToContents();
    selectedCabinetId = -1;
}

void PickupCabinetView::onAddCabinetClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加柜子");
    QFormLayout form(&dialog);
    QLineEdit locationEdit;
    QSpinBox capacitySpin;
    capacitySpin.setMinimum(1);
    capacitySpin.setMaximum(1000);
    form.addRow("位置:", &locationEdit);
    form.addRow("容量:", &capacitySpin);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        QString location = locationEdit.text().trimmed();
        int capacity = capacitySpin.value();
        if (location.isEmpty()) {
            QMessageBox::warning(this, "提示", "位置不能为空");
            return;
        }
        if (PickupCabinetController::instance().addCabinet(location, capacity)) {
            QMessageBox::information(this, "成功", "添加柜子成功");
        } else {
            QMessageBox::warning(this, "失败", "添加柜子失败");
        }
        refreshCabinetTable();
    }
}

void PickupCabinetView::onRemoveCabinetClicked() {
    if (selectedCabinetId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要删除的柜子");
        return;
    }
    if (PickupCabinetController::instance().removeCabinet(selectedCabinetId)) {
        QMessageBox::information(this, "成功", "删除柜子成功");
    } else {
        QMessageBox::warning(this, "失败", "删除柜子失败");
    }
    refreshCabinetTable();
}

void PickupCabinetView::onUpdateCabinetClicked() {
    if (selectedCabinetId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要修改的柜子");
        return;
    }
    // 获取当前柜子信息
    PickupCabinet cab;
    auto cabinets = PickupCabinetController::instance().getAllCabinets();
    for (const auto &c: cabinets) {
        if (c.cabinetId == selectedCabinetId) {
            cab = c;
            break;
        }
    }
    QDialog dialog(this);
    dialog.setWindowTitle("修改柜子信息");
    QFormLayout form(&dialog);
    QLineEdit locationEdit;
    locationEdit.setText(cab.location);
    QSpinBox capacitySpin;
    capacitySpin.setMinimum(1);
    capacitySpin.setMaximum(1000);
    capacitySpin.setValue(cab.capacity);
    form.addRow("位置:", &locationEdit);
    form.addRow("容量:", &capacitySpin);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        QString newLocation = locationEdit.text().trimmed();
        int newCapacity = capacitySpin.value();
        if (PickupCabinetController::instance().updateCabinetInfo(selectedCabinetId, newLocation, newCapacity)) {
            QMessageBox::information(this, "成功", "修改柜子信息成功");
        } else {
            QMessageBox::warning(this, "失败", "修改柜子信息失败");
        }
        refreshCabinetTable();
    }
}

void PickupCabinetView::onCabinetTableItemClicked(int row, int) {
    QTableWidgetItem *idItem = ui->cabinetTableWidget->item(row, 0);
    if (!idItem) return;
    selectedCabinetId = idItem->text().toInt();
}

void PickupCabinetView::onViewPackagesClicked() {
    if (selectedCabinetId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要查看的柜子");
        return;
    }
    // 获取当前柜子信息
    PickupCabinet cab;
    auto cabinets = PickupCabinetController::instance().getAllCabinets();
    for (const auto &c: cabinets) {
        if (c.cabinetId == selectedCabinetId) {
            cab = c;
            break;
        }
    }
    // 查询该柜所有包裹
    auto pkgs = PackageController::instance().queryByLocationAndStatus(cab.location, PackageStatus::STORED);
    QDialog dialog(this);
    dialog.setWindowTitle(QString("柜子[%1]的包裹").arg(cab.location));
    QVBoxLayout layout(&dialog);
    QTableWidget table(&dialog);
    table.setColumnCount(6);
    table.setHorizontalHeaderLabels({"ID", "体积", "易碎", "公司ID", "收件人ID"});
    table.setRowCount(pkgs.size());
    for (int i = 0; i < pkgs.size(); ++i) {
        const Package &p = pkgs[i];
        table.setItem(i, 0, new QTableWidgetItem(QString::number(p.packageId)));
        table.setItem(i, 1, new QTableWidgetItem(QString::number(p.volume)));
        table.setItem(i, 2, new QTableWidgetItem(p.isFragile ? "是" : "否"));
        table.setItem(i, 3, new QTableWidgetItem(QString::number(p.expressCompanyId)));
        table.setItem(i, 4, new QTableWidgetItem(QString::number(p.recipientId)));
    }
    table.resizeColumnsToContents();
    layout.addWidget(&table);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok, &dialog);
    layout.addWidget(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    dialog.exec();
}
