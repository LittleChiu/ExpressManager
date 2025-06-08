#pragma once
#include <QWidget>

namespace Ui {
class FeedbackView;
}

class FeedbackView : public QWidget {
    Q_OBJECT
public:
    explicit FeedbackView(int userId, QWidget *parent = nullptr);
    ~FeedbackView();
private slots:
    void refreshFeedbackTable();
    void onSubmitFeedbackClicked();
    void onViewPackageClicked();
private:
    Ui::FeedbackView *ui;
    int currentUserId = -1;
    int selectedFeedbackId = -1;
    bool isRecipient = false;
}; 