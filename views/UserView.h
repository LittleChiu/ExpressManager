#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>

class UserView : public QWidget {
    Q_OBJECT
public:
    explicit UserView(int userId, QWidget *parent = nullptr);
    void addTab(QWidget* widget, const QString& title);
    int getUserId() const { return userId; }
    QTabWidget* getTabWidget() const { return tabWidget; }
signals:
    void logoutRequested();
    void switchAccountRequested();
protected:
    int userId;
    QTabWidget* tabWidget;
    QPushButton* userMenuButton;
}; 