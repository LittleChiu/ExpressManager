#pragma once
#include <QWidget>

namespace Ui {
    class PickupCabinetView;
}

class PickupCabinetView : public QWidget {
    Q_OBJECT

public:
    explicit PickupCabinetView(QWidget *parent = nullptr);

    ~PickupCabinetView();

private slots:
    void refreshCabinetTable();

    void onAddCabinetClicked();

    void onRemoveCabinetClicked();

    void onUpdateCabinetClicked();

    void onCabinetTableItemClicked(int row, int column);

    void onViewPackagesClicked();

private:
    Ui::PickupCabinetView *ui;
    int selectedCabinetId = -1;
};
