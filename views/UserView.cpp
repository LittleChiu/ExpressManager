#include "UserView.h"
#include "UserController.h"
#include "User.h"
#include <QVBoxLayout>
#include <QMenu>

UserView::UserView(int userId, QWidget *parent)
    : QWidget(parent), userId(userId)
{
    this->resize(598, 354);
    this->setWindowTitle("Express");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // 用户名按钮
    User user = UserController::instance().getUserById(userId);
    userMenuButton = new QPushButton(user.username + " ▼", this);
    tabWidget->setCornerWidget(userMenuButton, Qt::TopRightCorner);

    QMenu* menu = new QMenu(this);
    QAction* logoutAction = menu->addAction("退出系统");
    QAction* switchAction = menu->addAction("更换账号");
    connect(userMenuButton, &QPushButton::clicked, this, [=]{
        menu->exec(userMenuButton->mapToGlobal(QPoint(0, userMenuButton->height())));
    });
    connect(logoutAction, &QAction::triggered, this, &UserView::logoutRequested);
    connect(switchAction, &QAction::triggered, this, &UserView::switchAccountRequested);
}

void UserView::addTab(QWidget* widget, const QString& title) {
    tabWidget->addTab(widget, title);
} 