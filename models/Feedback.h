#pragma once
#include <QString>
#include <QList>
#include <QSqlQuery>

class Feedback {
public:
    int feedbackId = -1;
    int packageId = -1;
    double rating = -1;
    QString comment;

    static Feedback fromQuery(const QSqlQuery &query) {
        Feedback fb;
        fb.feedbackId = query.value("feedbackId").toInt();
        fb.packageId = query.value("packageId").toInt();
        fb.rating = query.value("rating").toDouble();
        fb.comment = query.value("comment").toString();
        return fb;
    }
};
