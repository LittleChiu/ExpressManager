#pragma once

#include <QDateEdit>
#include <Package.h>
#include <ExpressCompany.h>
#include <QComboBox>
#include <QLabel>


namespace Ui {
    class StatisticsView;
}
class StatisticsView : public QWidget {
    Q_OBJECT
public:
    explicit StatisticsView(QWidget *parent = nullptr);


    private slots:
        void onSearchClicked();
        void onRefreshClicked();

private:
    Ui::StatisticsView *ui;
    QList<Package> packageList;
    QList<ExpressCompany> companyList;

    void updateStatistics();
    void refreshAll();
};

