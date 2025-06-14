#include "QuickPickupView.h"
#include "ui_QuickPickupView.h"
#include "PackageController.h"
#include "UserController.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

#include "Feedback.h"
#include "FeedbackController.h"

QuickPickupView::QuickPickupView(QWidget *parent) : QWidget(parent), ui(new Ui::QuickPickupView) {
    ui->setupUi(this);
    connect(ui->pickupButton, &QPushButton::clicked, this, &QuickPickupView::onPickupClicked);
    connect(ui->pickupCodeEdit, &QLineEdit::returnPressed, this, &QuickPickupView::onPickupClicked);
}

QuickPickupView::~QuickPickupView() {
    delete ui;
}

void QuickPickupView::onPickupClicked() {
    QString code = ui->pickupCodeEdit->text().trimmed();
    if (code.length() != 6) {
        showResult(false, "请输入6位取件码");
        return;
    }
    // 查找包裹
    auto pkgs = PackageController::instance().getAllPackages();
    for (const auto& pkg : pkgs) {
        if (pkg.pickupCode == code && pkg.status == PackageStatus::STORED) {
            // 弹出包裹信息，确认后才取件
            User recipient = UserController::instance().getUserById(pkg.recipientId);
            QString recipientStr = recipient.username.isEmpty() ? "-" : QString("%1(%2)").arg(recipient.username).arg(std::to_string(recipient.id));
            User expressman = UserController::instance().getUserById(pkg.expressmanId);
            QString expressmanStr = expressman.username.isEmpty() ? "-" : QString("%1(%2)").arg(expressman.username)
            .arg(std::to_string(expressman.id));
            QString info = QString("包裹ID: %1\n体积: %2\n易碎: %3\n状态: %4\n入柜时间: %5\n公司ID: %6\n收件人: %7\n快递员: %8")
                .arg(std::to_string(pkg.packageId))
                .arg(std::to_string(pkg.volume))
                .arg(pkg.isFragile ? "是" : "否")
                .arg((pkg.status == PackageStatus::STORED) ? "已存入柜" : (pkg.status == PackageStatus::PICKUPED ? "已取件" : "异常"))
                .arg(QDateTime::fromSecsSinceEpoch(pkg.storedTime).toString("yyyy-MM-dd HH:mm:ss"))
                .arg(std::to_string(pkg.expressCompanyId))
                .arg(recipientStr)
                .arg(expressmanStr);
            int ret = QMessageBox::question(this, "确认取件", info + "\n\n是否确认取件？", QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                bool ok1 = PackageController::instance().updateStatus(pkg.packageId, PackageStatus::PICKUPED);
                bool ok2 = PackageController::instance().updatePickupTime(pkg.packageId, QDateTime::currentSecsSinceEpoch());
                if (ok1 && ok2) {
                    Feedback fb;
                    fb.packageId = pkg.packageId;
                    fb.rating = -1;
                    fb.comment = "尚未评价";
                    FeedbackController::instance().addFeedback(fb);
                    showResult(true, "取件成功, 记得评价!");
                } else {
                    showResult(false, "取件失败，请重试");
                }
            }
            return;
        }
    }
    showResult(false, "取件码无效或包裹已被取出");
}

void QuickPickupView::showResult(bool success, const QString &msg) {
    QMessageBox::information(this, success ? "成功" : "失败", msg);
    if (success) ui->pickupCodeEdit->clear();
} 