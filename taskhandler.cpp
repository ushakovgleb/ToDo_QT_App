#include "taskhandler.h"

taskHandler::taskHandler() {
    db.open();
    loadTasks();
}

void taskHandler::deleteTask(int id) {
    db.deleteTask(id);
    loadTasks();
}
