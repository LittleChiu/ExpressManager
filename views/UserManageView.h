#pragma once
#include <QWidget>

namespace Ui {
class UserManageView;
}

class UserManageView : public QWidget {
    Q_OBJECT
public:
    explicit UserManageView(QWidget *parent = nullptr);
    ~UserManageView();

private slots:
    void refreshUserTable();
    void onAddUserClicked();
    void onModifyUserClicked();
    void onDeleteUserClicked();
    void onResetPasswordClicked();
    void onUserTableItemClicked(int row, int column);
    void onSelectExpressCompanyClicked();

private:
    Ui::UserManageView *ui;
    int selectedUserId = -1;
    int selectedCompanyId = -1;
    QString selectedCompanyName;
    QStringList selectedCompanyNames;
    QString selectedCompanyIdsStr;
}; 