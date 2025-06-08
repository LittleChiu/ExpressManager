#pragma once
#include <QSqlDatabase>

class DatabaseManager {
public:
    static QSqlDatabase& getDatabase();
    static void initDatabase();
}; 