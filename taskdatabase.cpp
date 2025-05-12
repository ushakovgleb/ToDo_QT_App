#include "taskdatabase.h"

TaskDatabase::TaskDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tasks.db");
}

bool TaskDatabase::open() {
    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных:" << db.lastError.text();
        return false;
    }

    init();
    return true;
}

void TaskDatabase::close() {
    db.close();
}

void TaskDatabase::init() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS tasks ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT,"
               "tags TEXT,"
               "deadline TEXT,"
               "category TEXT,"
               "completed INTEGER)");
}
