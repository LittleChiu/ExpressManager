#include "AdminView.h"
#include "ui_AdminView.h"
#include "ExpressCompanyController.h"
#include "UserController.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QVBoxLayout>

AdminView::AdminView(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminView)
{
    ui->setupUi(this);
    // 嵌入ExpressCompanyView
    expressCompanyView = new ExpressCompanyView(this);
    QVBoxLayout* layoutCompany = new QVBoxLayout;
    layoutCompany->addWidget(expressCompanyView);
    QWidget* tabCompany = ui->tabWidget->findChild<QWidget*>("tabCompany");
    if (tabCompany) {
        tabCompany->setLayout(layoutCompany);
    }
    // 嵌入UserManageView
    userManageView = new UserManageView(this);
    QVBoxLayout* layoutUser = new QVBoxLayout;
    layoutUser->addWidget(userManageView);
    QWidget* tabUser = ui->tabWidget->findChild<QWidget*>("tabUser");
    if (tabUser) {
        tabUser->setLayout(layoutUser);
    }
    // 嵌入PackageView
    packageView = new PackageView(userId, this);
    QVBoxLayout* layoutPackage = new QVBoxLayout;
    layoutPackage->addWidget(packageView);
    QWidget* tabPackage = ui->tabWidget->findChild<QWidget*>("tabPackage");
    if (tabPackage) {
        tabPackage->setLayout(layoutPackage);
    }
    // 嵌入PickupCabinetView
    cabinetView = new PickupCabinetView(this);
    QVBoxLayout* layoutCabinet = new QVBoxLayout;
    layoutCabinet->addWidget(cabinetView);
    QWidget* tabCabinet = ui->tabWidget->findChild<QWidget*>("tabCabinet");
    if (tabCabinet) {
        tabCabinet->setLayout(layoutCabinet);
    }
    // 嵌入UserProfileView
    userProfileView = new UserProfileView(this);
    userProfileView->setUserId(userId);
    QVBoxLayout* layoutProfile = new QVBoxLayout;
    layoutProfile->addWidget(userProfileView);
    QWidget* tabProfile = ui->tabWidget->findChild<QWidget*>("tabProfile");
    if (tabProfile) {
        tabProfile->setLayout(layoutProfile);
    }
}

AdminView::~AdminView()
{
    delete ui;
} 