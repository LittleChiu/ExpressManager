#include "Feedback.h"
#include <QVariant>

bool Feedback::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS Feedback ("
                      "feedbackId INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "packageId INTEGER,"
                      "rating REAL,"
                      "comment TEXT)");
}

Feedback Feedback::fromQuery(const QSqlQuery& query) {
    Feedback fb;
    fb.feedbackId = query.value("feedbackId").toInt();
    fb.packageId = query.value("packageId").toInt();
    fb.rating = query.value("rating").toDouble();
    fb.comment = query.value("comment").toString();
    return fb;
}

QList<Feedback> Feedback::getAll() {
    QList<Feedback> list;
    QSqlQuery query("SELECT * FROM Feedback");
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
}

bool Feedback::addFeedback(const Feedback& fb) {
    QSqlQuery query;
    query.prepare("INSERT INTO Feedback (packageId, rating, comment) VALUES (?, ?, ?)");
    query.addBindValue(fb.packageId);
    query.addBindValue(fb.rating);
    query.addBindValue(fb.comment);
    return query.exec();
}

QList<Feedback> Feedback::queryByPackage(int packageId) {
    QList<Feedback> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Feedback WHERE packageId = ?");
    query.addBindValue(packageId);
    query.exec();
    while (query.next()) {
        list.append(fromQuery(query));
    }
    return list;
} 