#include "taskhandler.h"

TaskHandler::TaskHandler() {
    if (!db.open()) {
        qDebug() << "Ошибка при открытии базы данных";
    } else {
        loadTasks();
    }

}

void TaskHandler::loadTasks() {
    tasksByCategory.clear();
    QList<Task> allTasks = db.loadTasks();
    for (const Task &t : allTasks) {
        tasksByCategory[t.category].append(t);
    }
}

int TaskHandler::addTask(const Task &task) {
    int id = db.addTask(task);
    Task newTask = task;
    newTask.id = id;
    tasksByCategory[task.category].append(newTask);
    return id;
}

void TaskHandler::updateTask(const Task &task) {
    db.updateTask(task);
    loadTasks();
}

void TaskHandler::deleteTask(int id) {
    db.deleteTask(id);
    loadTasks();
}

QList<Task> TaskHandler::getTasksByCategory(const QString &category) const {
    return tasksByCategory.value(category);
}
