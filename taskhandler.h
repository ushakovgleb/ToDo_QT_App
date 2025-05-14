#ifndef TASKHANDLER_H
#define TASKHANDLER_H


#include "taskdatabase.h"
#include <QMap>
#include <QString>


class TaskHandler
{
public:
    TaskHandler();

    void loadTasks();
    int addTask(const Task &task);
    void updateTask(const Task &task);
    void deleteTask(int id);
    QList<Task> getTasksByCategory(const QString &category) const;

private:
    TaskDatabase db;
    QMap<QString, QList<Task>> tasksByCategory;
};

#endif // TASKHANDLER_H
