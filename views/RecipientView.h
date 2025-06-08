#pragma once
#include "UserView.h"
#include "PickupView.h"
#include "FeedbackView.h"
#include "UserProfileView.h"

namespace Ui {
class RecipientView;
}

class RecipientView : public UserView {
    Q_OBJECT
public:
    explicit RecipientView(int userId, QWidget *parent = nullptr)
        : UserView(userId, parent) {
        addTab(new PickupView(userId, this), "取件");
        addTab(new FeedbackView(userId, this), "反馈评价");
        auto profile = new UserProfileView(this);
        profile->setUserId(userId);
        addTab(profile, "个人信息维护");
    }
    ~RecipientView() override = default;

}; 