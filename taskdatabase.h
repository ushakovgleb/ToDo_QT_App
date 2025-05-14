#ifndef TASKDATABASE_H
#define TASKDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QDebug>
#include <QList>
#include <QString>


struct Task {
    int id;
    QString name;
    QString tags;
    QString category;
    QDate deadline;
    bool completed;
};

class TaskDatabase
{
public:
    TaskDatabase();
    bool open();
    void close();
    bool addTask(const Task &task);
    QList<Task> loadTasks();
    bool updateTask(const Task &task);
    bool deleteTask(int id);

private:
    QSqlDatabase db;
    void init();
};

#endif // TASKDATABASE_H
