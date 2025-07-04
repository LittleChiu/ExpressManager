#pragma once
#include "UserView.h"
#include "PackageView.h"
#include "UserProfileView.h"
#include "FeedbackView.h"

namespace Ui {
class ExpressmanView;
}

class ExpressmanView : public UserView {
    Q_OBJECT
public:
    explicit ExpressmanView(int userId, QWidget *parent = nullptr)
        : UserView(userId, parent) {
        addTab(new PackageView(userId, this), "包裹管理");
        addTab(new FeedbackView(userId, this), "反馈评价");
        auto profile = new UserProfileView(this);
        profile->setUserId(userId);
        addTab(profile, "个人信息维护");
    }

}; 