#pragma once
#include <QWidget>
#include "User.h"
#include "PackageView.h"
#include "PickupCabinetView.h"
#include "ExpressCompanyView.h"
#include "UserManageView.h"
#include "UserProfileView.h"

namespace Ui {
class AdminView;
}

class AdminView : public QWidget {
    Q_OBJECT
public:
    explicit AdminView(int userId, QWidget *parent = nullptr);
    ~AdminView();


private:
    Ui::AdminView *ui;
    int selectedUserId = -1;
    PackageView* packageView = nullptr;
    PickupCabinetView* cabinetView = nullptr;
    ExpressCompanyView* expressCompanyView = nullptr;
    UserManageView* userManageView = nullptr;
    UserProfileView* userProfileView = nullptr;
}; 