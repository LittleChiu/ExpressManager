#include "FeedbackController.h"

bool FeedbackController::addFeedback(const Feedback& fb) {
    return Feedback::addFeedback(fb);
}

QList<Feedback> FeedbackController::getAllFeedbacks() {
    return Feedback::getAll();
}

QList<Feedback> FeedbackController::getFeedbacksByPackage(int packageId) {
    return Feedback::queryByPackage(packageId);
} 