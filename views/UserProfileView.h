#pragma once
#include <QWidget>

namespace Ui {
class UserProfileView;
}

class UserProfileView : public QWidget {
    Q_OBJECT
public:
    explicit UserProfileView(QWidget *parent = nullptr);
    ~UserProfileView();
    void setUserId(int userId); // 设置当前用户

private slots:
    void onUpdatePhoneClicked();
    void onUpdatePasswordClicked();

private:
    Ui::UserProfileView *ui;
    int userId = -1;
}; 