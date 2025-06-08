#pragma once
#include <QObject>
#include "Feedback.h"

class FeedbackController : public QObject {
    Q_OBJECT
public:
    static FeedbackController& instance() {
        static FeedbackController instance;
        return instance;
    }
    bool addFeedback(const Feedback& fb);
    QList<Feedback> getAllFeedbacks();
    QList<Feedback> getFeedbacksByPackage(int packageId);
private:
    FeedbackController() {}
}; 