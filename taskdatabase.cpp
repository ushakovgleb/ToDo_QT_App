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

bool TaskDatabase::addTask(const Task &task) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (name, tags, deadline, category, completed)"
                  "VALUES (:name, :tags, :deadline, :category, :completed)");


    query.addBindValue(task.name);
    query.addBindValue(task.tags);
    query.addBindValue(task.deadline.toString(Qt::ISODate));
    query.addBindValue(task.category);
    query.addBindValue(task.completed ? 1 : 0);
    return query.exec();
}

QList<Task> TaskDatabase::loadTasks() {
    QList<Task> list;
    QSqlQuery query("SELECT id, name, tags, deadline, category, completed FROM tasks");
    while (query.next()) {
        Task tsk;
        tsk.id = query.value(0).toInt();
        tsk.name = query.value(1).toString();
        tsk.tags = query.value(2).toString();
        tsk.deadline = QDate::fromString(query.value(3).toString(), Qt::ISODate);
        tsk.category = query.value(4).toString();
        tsk.completed = query.value(5).toInt() == 1;
        list.append(tsk);
    }

    return list;
}
