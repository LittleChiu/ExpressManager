#pragma once
#include <QString>
#include <QList>
#include <QSqlQuery>

class Feedback {
public:
    int feedbackId;
    int packageId;
    double rating;
    QString comment;

    static bool createTable();
    static Feedback fromQuery(const QSqlQuery& query);
    static QList<Feedback> getAll();
    static bool addFeedback(const Feedback& fb);
    static QList<Feedback> queryByPackage(int packageId);
}; 