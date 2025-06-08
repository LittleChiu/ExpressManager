#pragma once
#include "UserView.h"
#include "PackageView.h"
#include "PickupCabinetView.h"
#include "ExpressCompanyView.h"
#include "UserManageView.h"
#include "UserProfileView.h"
#include "PickupView.h"
#include "FeedbackView.h"

namespace Ui {
class AdminView;
}

class AdminView : public UserView {
    Q_OBJECT
public:
    explicit AdminView(int userId, QWidget *parent = nullptr)
        : UserView(userId, parent) {
        addTab(new ExpressCompanyView(this), "快递公司管理");
        addTab(new UserManageView(this), "用户管理");
        addTab(new PackageView(userId, this), "包裹管理");
        addTab(new PickupCabinetView(this), "快递柜管理");
        addTab(new PickupView(userId, this), "取件管理");
        addTab(new FeedbackView(userId, this), "反馈评价");
        auto profile = new UserProfileView(this);
        profile->setUserId(userId);
        addTab(profile, "个人信息");
    }
}; 