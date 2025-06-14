#include "DatabaseManager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include "ExpressCompany.h"
#include "ExpressCompanyController.h"
#include "FeedbackController.h"

#include "PackageController.h"
#include "PickupCabinetController.h"
#include "UserController.h"

QSqlDatabase& DatabaseManager::getDatabase() {
    static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
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
    UserController::instance().createTable();
    ExpressCompanyController::instance().createTable();
    PickupCabinetController::instance().createTable();
    PackageController::instance().createTable();
    FeedbackController::instance().createTable();
}
