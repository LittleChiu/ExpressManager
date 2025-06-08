#include "ExpressmanView.h"
#include "ui_ExpressmanView.h"
#include <QVBoxLayout>

ExpressmanView::ExpressmanView(int userId, QWidget *parent)
    : QWidget(parent), ui(new Ui::ExpressmanView), userId(userId)
{
    ui->setupUi(this);
    // 嵌入PackageView
    packageView = new PackageView(userId, this);
    QVBoxLayout* layoutPackage = new QVBoxLayout;
    layoutPackage->addWidget(packageView);
    QWidget* tabPackage = ui->tabWidget->findChild<QWidget*>("tabPackage");
    if (tabPackage) {
        tabPackage->setLayout(layoutPackage);
    }
    // 嵌入UserProfileView
    profileView = new UserProfileView(this);
    profileView->setUserId(userId);
    QVBoxLayout* layoutProfile = new QVBoxLayout;
    layoutProfile->addWidget(profileView);
    QWidget* tabProfile = ui->tabWidget->findChild<QWidget*>("tabProfile");
    if (tabProfile) {
        tabProfile->setLayout(layoutProfile);
    }
    // 嵌入FeedbackView
    feedbackView = new FeedbackView(userId, this);
    QVBoxLayout* layoutFeedback = new QVBoxLayout;
    layoutFeedback->addWidget(feedbackView);
    QWidget* tabFeedback = ui->tabWidget->findChild<QWidget*>("tabFeedback");
    if (tabFeedback) {
        tabFeedback->setLayout(layoutFeedback);
    }
}

ExpressmanView::~ExpressmanView() {
    delete ui;
}