#include "StatisticsView.h"

#include "ExpressCompanyController.h"
#include "PackageController.h"
#include "ui_StatisticsView.h"

StatisticsView::StatisticsView(QWidget *parent)
    : QWidget(parent), ui(new Ui::StatisticsView)
{
    ui->setupUi(this);
    connect(ui->searchButton, &QPushButton::clicked, this, &StatisticsView::onSearchClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &StatisticsView::onRefreshClicked);
    // 可选：设置表头
    ui->statisticsTableWidget->setColumnCount(3);
    QStringList headers = {"类型", "数量", "备注"};
    ui->statisticsTableWidget->setHorizontalHeaderLabels(headers);
    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate());
    refreshAll();
}


void StatisticsView::onSearchClicked(){
    updateStatistics();
}
void StatisticsView::onRefreshClicked(){

    refreshAll();
}
void StatisticsView::refreshAll() {
    packageList = PackageController::instance().getAllPackages();
    companyList = ExpressCompanyController::instance().getAllCompanies();
    ui->companyComboBox->clear();
    ui->companyComboBox->addItem("全部公司", -1);
    for (const auto &c : companyList) {
        ui->companyComboBox->addItem(c.name, c.companyId);
    }
}
void StatisticsView::updateStatistics()
{
    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();
    int companyId = ui->companyComboBox->currentData().toInt();

    int totalReceived = 0;
    int totalPicked = 0;
    int totalUnpicked = 0;
    double totalAmount = 0.0;

    for (const auto &pkg : packageList) {
        QDate stored = QDateTime::fromSecsSinceEpoch(pkg.storedTime).date();
        if (stored < startDate || stored > endDate)
            continue;
        if (companyId != -1 && pkg.expressCompanyId != companyId)
            continue;

        totalReceived++;
        if (pkg.status == PackageStatus::PICKUPED)
            totalPicked++;
        else
            totalUnpicked++;

        // 统计费用
        if (pkg.status == PackageStatus::STORED || pkg.status == PackageStatus::PICKUPED) {
            for (const auto &comp : companyList) {
                if (comp.companyId == pkg.expressCompanyId)
                    totalAmount += comp.packageFee;
            }
        }
    }

    ui->statisticsTableWidget->setRowCount(3);
    ui->statisticsTableWidget->setItem(0, 0, new QTableWidgetItem("收件量"));
    ui->statisticsTableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(totalReceived)));
    ui->statisticsTableWidget->setItem(0, 2, new QTableWidgetItem("统计期间内所有收件"));

    ui->statisticsTableWidget->setItem(1, 0, new QTableWidgetItem("取件量"));
    ui->statisticsTableWidget->setItem(1, 1, new QTableWidgetItem(QString::number(totalPicked)));
    ui->statisticsTableWidget->setItem(1, 2, new QTableWidgetItem("已取出的件数"));

    ui->statisticsTableWidget->setItem(2, 0, new QTableWidgetItem("未取件量"));
    ui->statisticsTableWidget->setItem(2, 1, new QTableWidgetItem(QString::number(totalUnpicked)));
    ui->statisticsTableWidget->setItem(2, 2, new QTableWidgetItem("未被取出的件数"));

    ui->totalAmountLabel->setText(tr("应结算金额：%1 元").arg(totalAmount, 0, 'f', 2));
}
