#pragma once
#include <QWidget>

namespace Ui {
    class QuickPickupView;
}

class QuickPickupView : public QWidget {
    Q_OBJECT

public:
    explicit QuickPickupView(QWidget *parent = nullptr);

    ~QuickPickupView();

private slots:
    void onPickupClicked();

private:
    Ui::QuickPickupView *ui;

    void showResult(bool success, const QString &msg);
};
