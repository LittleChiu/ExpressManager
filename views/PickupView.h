#pragma once
#include <QWidget>

namespace Ui {
class PickupView;
}

class PickupView : public QWidget {
    Q_OBJECT
public:
    explicit PickupView(int userId, QWidget *parent = nullptr);
    ~PickupView();
private slots:
    void refreshPickupTable();
    void onPickupClicked();
private:
    Ui::PickupView *ui;
    int currentUserId = -1;
    int selectedPackageId = -1;
}; 