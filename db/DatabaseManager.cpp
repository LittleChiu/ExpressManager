#include "DatabaseManager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "ExpressCompany.h"
#include "Feedback.h"
#include "User.h"
#include "PickupCabinet.h"
#include "Package.h"

QSqlDatabase& DatabaseManager::getDatabase() {
    static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("express_cabinet.db");
    return db;
}

void DatabaseManager::initDatabase() {
    QSqlDatabase db = getDatabase();

    // 检查驱动是否可用
    qDebug() << "可用驱动:" << QSqlDatabase::drivers();

    if (!db.isValid()) {
        qDebug() << "数据库驱动无效";
        return;
    }

    if (!db.open()) {
        qDebug() << "无法打开数据库";
        return;
    }

    // 自动建表
    User::createTable();
    ExpressCompany::createTable();
    PickupCabinet::createTable();
    Package::createTable();
    Feedback::createTable();
}
