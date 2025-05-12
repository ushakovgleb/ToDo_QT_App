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

private:
    TaskDatabase db;
    QMap<QString, QList<Task> tasksByCategory;
};

#endif // TASKHANDLER_H
