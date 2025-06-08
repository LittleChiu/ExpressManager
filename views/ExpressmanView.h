#pragma once
#include <QWidget>
#include "PackageView.h"
#include "UserProfileView.h"

namespace Ui {
class ExpressmanView;
}

class ExpressmanView : public QWidget {
    Q_OBJECT
public:
    explicit ExpressmanView(int userId, QWidget *parent = nullptr);
    ~ExpressmanView();
private:
    Ui::ExpressmanView *ui;
    PackageView* packageView = nullptr;
    UserProfileView* profileView = nullptr;
    int userId = -1;
}; 