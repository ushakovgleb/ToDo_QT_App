#ifndef TASKHANDLER_H
#define TASKHANDLER_H


#include "taskdatabase.h"
#include <Qlist>
#include <QMap>
#include <QString>


class taskHandler
{
public:
    taskHandler();

    void loadTasks();
    int addTask(const Task &task);
    void updateTask(const Task &task);
    void deleteTask(int id);


private:
    TaskDatabase db;
    QMap<QString, QList<Task> tasksByCategory;
};

#endif // TASKHANDLER_H
