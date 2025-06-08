#include "ExpressCompanyView.h"
#include "ui_ExpressCompanyView.h"
#include "ExpressCompanyController.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

ExpressCompanyView::ExpressCompanyView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ExpressCompanyView)
{
    ui->setupUi(this);
    connect(ui->addCompanyButton, &QPushButton::clicked, this, &ExpressCompanyView::onAddCompanyClicked);
    connect(ui->removeCompanyButton, &QPushButton::clicked, this, &ExpressCompanyView::onRemoveCompanyClicked);
    connect(ui->updateFeeButton, &QPushButton::clicked, this, &ExpressCompanyView::onUpdateFeeClicked);
    connect(ui->refreshCompanyButton, &QPushButton::clicked, this, &ExpressCompanyView::refreshCompanyList);
    connect(ui->companyListWidget, &QListWidget::itemClicked, this, &ExpressCompanyView::onCompanyListItemClicked);
    refreshCompanyList();
}

ExpressCompanyView::~ExpressCompanyView() {
    delete ui;
}

void ExpressCompanyView::onAddCompanyClicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("添加公司");
    QFormLayout form(&dialog);
    QLineEdit nameEdit;
    QDoubleSpinBox feeSpin;
    feeSpin.setPrefix("￥");
    feeSpin.setDecimals(2);
    feeSpin.setMaximum(9999.99);
    form.addRow("公司名称:", &nameEdit);
    form.addRow("费用:", &feeSpin);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit.text().trimmed();
        double fee = feeSpin.value();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "提示", "公司名称不能为空");
            return;
        }
        if (ExpressCompanyController::instance().addCompany(name, fee)) {
            QMessageBox::information(this, "成功", "添加公司成功");
        } else {
            QMessageBox::warning(this, "失败", "添加公司失败，可能已存在");
        }
        refreshCompanyList();
    }
}

void ExpressCompanyView::onUpdateFeeClicked() {
    if (selectedCompanyId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要修改的公司");
        return;
    }
    auto companies = ExpressCompanyController::instance().getAllCompanies();
    double oldFee = 0.0;
    QString companyName;
    for (const auto& c : companies) {
        if (c.companyId == selectedCompanyId) {
            oldFee = c.packageFee;
            companyName = c.name;
            break;
        }
    }
    QDialog dialog(this);
    dialog.setWindowTitle("修改费用");
    QFormLayout form(&dialog);
    QLineEdit nameEdit;
    nameEdit.setText(companyName);
    nameEdit.setReadOnly(true);
    QDoubleSpinBox feeSpin;
    feeSpin.setPrefix("￥");
    feeSpin.setDecimals(2);
    feeSpin.setMaximum(9999.99);
    feeSpin.setValue(oldFee);
    form.addRow("公司名称:", &nameEdit);
    form.addRow("新费用:", &feeSpin);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    if (dialog.exec() == QDialog::Accepted) {
        double newFee = feeSpin.value();
        if (ExpressCompanyController::instance().updateFee(selectedCompanyId, newFee)) {
            QMessageBox::information(this, "成功", "修改费用成功");
        } else {
            QMessageBox::warning(this, "失败", "修改费用失败");
        }
        refreshCompanyList();
    }
}

void ExpressCompanyView::onRemoveCompanyClicked() {
    if (selectedCompanyId == -1) {
        QMessageBox::warning(this, "提示", "请先选择要删除的公司");
        return;
    }
    if (ExpressCompanyController::instance().removeCompany(selectedCompanyId)) {
        QMessageBox::information(this, "成功", "删除公司成功");
    } else {
        QMessageBox::warning(this, "失败", "删除公司失败");
    }
    refreshCompanyList();
}

void ExpressCompanyView::refreshCompanyList() {
    ui->companyListWidget->clear();
    auto companies = ExpressCompanyController::instance().getAllCompanies();
    for (const auto& c : companies) {
        QListWidgetItem* item = new QListWidgetItem(QString("%1（￥%2）").arg(c.name).arg(c.packageFee, 0, 'f', 2));
        item->setData(Qt::UserRole, c.companyId);
        ui->companyListWidget->addItem(item);
    }
    selectedCompanyId = -1;
}

void ExpressCompanyView::onCompanyListItemClicked(QListWidgetItem* item) {
    if (!item) return;
    selectedCompanyId = item->data(Qt::UserRole).toInt();
} 