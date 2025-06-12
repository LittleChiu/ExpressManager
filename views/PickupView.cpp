#include "PickupView.h"

#include <QDateTime>

#include "ui_PickupView.h"
#include "PackageController.h"
#include "FeedbackController.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QPushButton>

PickupView::PickupView(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::PickupView), currentUserId(userId)
{
    ui->setupUi(this);
    connect(ui->pickupButton, &QPushButton::clicked, this, &PickupView::onPickupClicked);
    connect(ui->pickupTableWidget, &QTableWidget::cellClicked, this, [this](int row, int){
        QTableWidgetItem* idItem = ui->pickupTableWidget->item(row, 0);
        if (idItem) selectedPackageId = idItem->text().toInt();
    });
    connect(ui->refreshPickupButton, &QPushButton::clicked, this, &PickupView::refreshPickupTable);
    refreshPickupTable();
}

PickupView::~PickupView() {
    delete ui;
}

void PickupView::refreshPickupTable() {
    QStringList headers = {"ID", "体积", "重量", "易碎", "状态", "位置", "公司ID", "快递员ID"};
    ui->pickupTableWidget->setColumnCount(headers.size());
    ui->pickupTableWidget->setHorizontalHeaderLabels(headers);
    ui->pickupTableWidget->setRowCount(0);
    auto pkgs = PackageController::instance().getAllPackages();
    int rowIdx = 0;
    for (const auto& p : pkgs) {
        if (p.recipientId != currentUserId) continue;
        if (p.status != PackageStatus::STORED) continue;
        ui->pickupTableWidget->insertRow(rowIdx);
        ui->pickupTableWidget->setItem(rowIdx, 0, new QTableWidgetItem(QString::number(p.packageId)));
        ui->pickupTableWidget->setItem(rowIdx, 1, new QTableWidgetItem(QString::number(p.volume)));
        ui->pickupTableWidget->setItem(rowIdx, 2, new QTableWidgetItem(QString::number(p.weight)));
        ui->pickupTableWidget->setItem(rowIdx, 3, new QTableWidgetItem(p.isFragile ? "是" : "否"));
        ui->pickupTableWidget->setItem(rowIdx, 4, new QTableWidgetItem("已存入柜"));
        ui->pickupTableWidget->setItem(rowIdx, 5, new QTableWidgetItem(p.location));
        ui->pickupTableWidget->setItem(rowIdx, 6, new QTableWidgetItem(QString::number(p.expressCompanyId)));
        ui->pickupTableWidget->setItem(rowIdx, 7, new QTableWidgetItem(QString::number(p.expressmanId)));
        rowIdx++;
    }
    ui->pickupTableWidget->resizeColumnsToContents();
    selectedPackageId = -1;
}

void PickupView::onPickupClicked() {
    if (selectedPackageId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要取件的包裹");
        return;
    }
    if (PackageController::instance().updateStatus(selectedPackageId, PackageStatus::PICKUPED)) {
        PackageController::instance().updatePickupTime(selectedPackageId,QDateTime::currentSecsSinceEpoch());
        Feedback fb;
        fb.packageId = selectedPackageId;
        fb.rating = 0.0;
        fb.comment = "";
        FeedbackController::instance().addFeedback(fb);
        QMessageBox::information(this, "成功", "取件成功, 记得评价!");
        refreshPickupTable();
    } else {
        QMessageBox::warning(this, "失败", "取件失败");
    }
} 