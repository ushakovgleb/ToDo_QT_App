#include "taskhandler.h"

taskHandler::taskHandler() {
    db.open();
    loadTasks();
}

void taskHandler::loadTasks() {
    tasksByCategory.clear();
    QList<Task> allTasks = db.loadAllTasks();
    for (const Task &t : allTasks) {
        tasksByCategory[t.category].append(t);
    }
}

int taskHandler::addTask(const Task &task) {
    int id = db.addTask(task);
    Task newTask = task;
    newTask.id = id;
    tasksByCategory[task.category].append(newTask);
    return id;
}

void taskHandler::deleteTask(int id) {
    db.deleteTask(id);
    loadTasks();
}
