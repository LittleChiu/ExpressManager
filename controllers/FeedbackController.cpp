#include "FeedbackController.h"

bool FeedbackController::addFeedback(const Feedback& fb) {
    QSqlQuery query;
    query.prepare("INSERT INTO Feedback (packageId, rating, comment) VALUES (?, ?, ?)");
    query.addBindValue(fb.packageId);
    query.addBindValue(fb.rating);
    query.addBindValue(fb.comment);
    return query.exec();
}

QList<Feedback> FeedbackController::getAllFeedbacks() {
    QList<Feedback> list;
    QSqlQuery query("SELECT * FROM Feedback");
    while (query.next()) {
        list.append(Feedback::fromQuery(query));
    }
    return list;
}

QList<Feedback> FeedbackController::getFeedbacksByPackage(int packageId) {
    QList<Feedback> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Feedback WHERE packageId = ?");
    query.addBindValue(packageId);
    query.exec();
    while (query.next()) {
        list.append(Feedback::fromQuery(query));
    }
    return list;
}
bool FeedbackController::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS Feedback ("
                      "feedbackId INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "packageId INTEGER,"
                      "rating REAL,"
                      "comment TEXT)");
}