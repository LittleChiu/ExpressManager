#pragma once
#include <QListWidgetItem>
#include <QWidget>

namespace Ui {
class ExpressCompanyView;
}

class ExpressCompanyView : public QWidget {
    Q_OBJECT
public:
    explicit ExpressCompanyView(QWidget *parent = nullptr);
    ~ExpressCompanyView();

private slots:
    void onAddCompanyClicked();
    void onRemoveCompanyClicked();
    void onUpdateFeeClicked();
    void refreshCompanyList();
    void onCompanyListItemClicked(QListWidgetItem* item);

private:
    Ui::ExpressCompanyView *ui;
    int selectedCompanyId = -1;
}; 