#include "FeedbackView.h"
#include "ui_FeedbackView.h"
#include "FeedbackController.h"
#include "PackageController.h"
#include "UserController.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QInputDialog>

FeedbackView::FeedbackView(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::FeedbackView), currentUserId(userId)
{
    ui->setupUi(this);
    // 判断身份
    User user = UserController::instance().getUserById(userId).value();
    isRecipient = (user.role == UserRole::RECIPIENT);
    connect(ui->refreshFeedbackButton, &QPushButton::clicked, this, &FeedbackView::refreshFeedbackTable);
    connect(ui->submitFeedbackButton, &QPushButton::clicked, this, &FeedbackView::onSubmitFeedbackClicked);
    connect(ui->viewPackageButton, &QPushButton::clicked, this, &FeedbackView::onViewPackageClicked);
    connect(ui->feedbackTableWidget, &QTableWidget::cellClicked, this, [this](int row, int){
        QTableWidgetItem* idItem = ui->feedbackTableWidget->item(row, 0);
        if (idItem) selectedFeedbackId = idItem->text().toInt();
    });
    refreshFeedbackTable();
}

FeedbackView::~FeedbackView() {
    delete ui;
}

void FeedbackView::refreshFeedbackTable() {
    QStringList headers = {"反馈ID", "包裹ID", "评分", "评论", "收件人", "派件人"};
    ui->feedbackTableWidget->setColumnCount(headers.size());
    ui->feedbackTableWidget->setHorizontalHeaderLabels(headers);
    ui->feedbackTableWidget->setRowCount(0);
    QList<Feedback> feedbacks;
    if (isRecipient) {
        // 收件人：只看自己收到的包裹的反馈
        auto pkgs = PackageController::instance().getAllPackages();
        QList<int> myPkgIds;
        for (const auto& p : pkgs) {
            if (p.recipientId == currentUserId) myPkgIds.append(p.packageId);
        }
        for (int pkgId : myPkgIds) {
            auto fbs = FeedbackController::instance().getFeedbacksByPackage(pkgId);
            feedbacks.append(fbs);
        }
    } else {
        // 快递员：只看自己投递的包裹的反馈
        auto pkgs = PackageController::instance().getAllPackages();
        QList<int> myPkgIds;
        for (const auto& p : pkgs) {
            if (p.expressmanId == currentUserId) myPkgIds.append(p.packageId);
        }
        for (int pkgId : myPkgIds) {
            auto fbs = FeedbackController::instance().getFeedbacksByPackage(pkgId);
            feedbacks.append(fbs);
        }
    }
    // 获取所有用户信息
    auto users = UserController::instance().getAllUsers();
    int rowIdx = 0;
    for (const auto& fb : feedbacks) {
        // 查找包裹
        Package pkg;
        auto pkgs = PackageController::instance().getAllPackages();
        for (const auto& p : pkgs) {
            if (p.packageId == fb.packageId) {
                pkg = p;
                break;
            }
        }
        // 查找收件人、派件人用户名
        QString recipientName, expressmanName;
        for (const auto& u : users) {
            if (u.id == pkg.recipientId) recipientName = u.username;
            if (u.id == pkg.expressmanId) expressmanName = u.username;
        }
        ui->feedbackTableWidget->insertRow(rowIdx);
        ui->feedbackTableWidget->setItem(rowIdx, 0, new QTableWidgetItem(QString::number(fb.feedbackId)));
        ui->feedbackTableWidget->setItem(rowIdx, 1, new QTableWidgetItem(QString::number(fb.packageId)));
        ui->feedbackTableWidget->setItem(rowIdx, 2, new QTableWidgetItem(QString::number(fb.rating, 'f', 1)));
        ui->feedbackTableWidget->setItem(rowIdx, 3, new QTableWidgetItem(fb.comment));
        ui->feedbackTableWidget->setItem(rowIdx, 4, new QTableWidgetItem(recipientName));
        ui->feedbackTableWidget->setItem(rowIdx, 5, new QTableWidgetItem(expressmanName));
        rowIdx++;
    }
    ui->feedbackTableWidget->resizeColumnsToContents();
    selectedFeedbackId = -1;
}

void FeedbackView::onSubmitFeedbackClicked() {
    if (selectedFeedbackId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要评价的反馈");
        return;
    }
    // 查找反馈对应包裹
    Feedback fb;
    auto all = FeedbackController::instance().getAllFeedbacks();
    for (const auto& f : all) {
        if (f.feedbackId == selectedFeedbackId) {
            fb = f;
            break;
        }
    }
    // 查找包裹收件人
    Package pkg;
    auto pkgs = PackageController::instance().getAllPackages();
    for (const auto& p : pkgs) {
        if (p.packageId == fb.packageId) {
            pkg = p;
            break;
        }
    }
    if (pkg.recipientId != currentUserId) {
        QMessageBox::warning(this, "提示", "只能评价自己的包裹");
        return;
    }
    if (fb.rating > 0.0) {
        QMessageBox::information(this, "提示", "该包裹已评价，不能重复评价");
        return;
    }
    bool ok = false;
    double rating = QInputDialog::getDouble(this, "评分", "请输入评分（0-5分）", 5.0, 0, 5, 1, &ok);
    if (!ok) return;
    QString comment = QInputDialog::getText(this, "评论", "请输入评价内容：", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    // 更新数据库
    QSqlQuery query;
    query.prepare("UPDATE Feedback SET rating = ?, comment = ? WHERE feedbackId = ?");
    query.addBindValue(rating);
    query.addBindValue(comment);
    query.addBindValue(selectedFeedbackId);
    if (query.exec()) {
        QMessageBox::information(this, "成功", "评价成功");
        refreshFeedbackTable();
    } else {
        QMessageBox::warning(this, "失败", "评价失败");
    }
}

void FeedbackView::onViewPackageClicked() {
    if (selectedFeedbackId == -1) {
        QMessageBox::warning(this, "提示", "请先选择一条反馈");
        return;
    }
    // 查找反馈对应包裹
    Feedback fb;
    auto all = FeedbackController::instance().getAllFeedbacks();
    for (const auto& f : all) {
        if (f.feedbackId == selectedFeedbackId) {
            fb = f;
            break;
        }
    }
    Package pkg;
    auto pkgs = PackageController::instance().getAllPackages();
    for (const auto& p : pkgs) {
        if (p.packageId == fb.packageId) {
            pkg = p;
            break;
        }
    }
    QString info = QString("包裹ID: %1\n体积: %2\n易碎: %4\n状态: %5\n位置: %6\n公司ID: %7\n收件人ID: %8\n快递员ID: %9")
        .arg(std::to_string(pkg.packageId)).arg(std::to_string(pkg.volume)).arg(pkg.isFragile ? "是" : "否")
        .arg((pkg.status == PackageStatus::STORED) ? "已存入柜" : (pkg.status == PackageStatus::PICKUPED ? "已取件" : "异常"))
        .arg(pkg.location).arg(std::to_string(pkg.expressCompanyId)).arg(std::to_string(pkg.recipientId))
    .arg(std::to_string(pkg.expressmanId));
    QMessageBox::information(this, "包裹信息", info);
} 