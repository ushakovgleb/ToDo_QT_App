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
