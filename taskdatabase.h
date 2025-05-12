#ifndef TASKDATABASE_H
#define TASKDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QDebug>


struct Task {
    int id;
    Qstring name;
    Qstring tags;
    Qstring category;
    QDate deadline;
    bool completed;
};

class TaskDatabase
{
public:
    TaskDatabase();
};

#endif // TASKDATABASE_H
