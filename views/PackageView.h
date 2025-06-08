#pragma once
#include <QWidget>

namespace Ui {
class PackageView;
}

class PackageView : public QWidget {
    Q_OBJECT
public:
    explicit PackageView(int userId = -1, QWidget *parent = nullptr);
    ~PackageView();

private slots:
    void refreshPackageTable();
    void onAddPackageClicked();
    void onUpdateStatusClicked();
    void onPackageTableItemClicked(int row, int column);

private:
    Ui::PackageView *ui;
    int selectedPackageId = -1;
    int currentUserId = -1;
}; 