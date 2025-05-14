#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskdatabase.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenu>
#include <QDate>

TaskDatabase db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ToDo App");
    resize(1000, 1000);

    ui->deadLineEdit->setDate(QDate::currentDate());

    categoryLists["Сегодня"] = ui->listToday;
    categoryLists["Завтра"] = ui->listTomorrow;
    categoryLists["След. неделя"] = ui->listNextWeek;
    categoryLists["Потом"] = ui->listLater;

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addTask);

    // Цикл по всем спискам задач
    for (auto list : categoryLists) {

        // Контекстное меню
        list->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(list, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

        connect(list, &QListWidget::itemChanged, this, [this](QListWidgetItem *item) {

            int id = item->data(Qt::UserRole).toInt();

            // Обновление стилей (при выполнении checkbox)
            QFont font = item->font();
            font.setStrikeOut(item->checkState() == Qt::Checked);
            item->setFont(font);

            // Создание объекта задачи с обновленными данными
            Task tsk;
            tsk.id = id;
            tsk.name = item->text().section("|", 0, 0).trimmed(); // Имя задачи
            tsk.tags = item->text().section("|", 1, 1).replace("🏷", "").trimmed(); // Теги задачи
            tsk.deadline = QDate::fromString(item->text().section("|", 2, 2).replace("⏳", "").trimmed(), "yyyy-MM-dd"); // Дата
            tsk.category = getCategoryOfItem(item); // Категория
            tsk.completed = item->checkState() == Qt::Checked; // Статус выполнения задачи


            taskHandler.updateTask(tsk);

        });

    }

    loadTasksToUI();
}

void MainWindow::addTask() {
    // Получение данных из полей
    QString name = ui->taskInput->text().trimmed();
    QString tags = ui->tagInput->text().trimmed();
    QDate date = ui->deadlineEdit->date();
    QString category = ui->categorySelect->currentText();

    if (name.isEmpty()) return;

    Task tsk;
    tsk.name = name;
    tsk.tags = tags;
    tsk.deadline = date;
    tsk.category = category;
    tsk.completed = false;

    int id = taskHandler.addTask(tsk);

    QString display = QString("%1  | 🏷 %2 | ⏳ %3").arg(name, tags, date.toString("yyyy-MM-dd"));
    QListWidgetItem * item = new QListWidgetItem(display);
    item->setCheckState(Qt::Unchecked);
    item->setData(Qt::UserRole, id);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    categoryLists[category]->addItem(item);

    ui->taskInput->clear();
    ui->tagInput->clear();
    ui->deadlineEdit->setDate(QDate::currentDate());
}

void MainWindow::removeSelectedTask(QListWidgetItem *item) {
    if (!item) return;

    int id = item->data(Qt::UserRole).toInt();
    taskHandler.deleteTask(id);
    delete item;
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QListWidget *senderList = qobject_cast<QListWidget*>(sender());
    if (!senderList) return;
    QListWidgetItem * item = senderList->itemAt(pos);
    if (!item) return;

    QMenu menu;
    QAction *remove = menu.addAction("Удалить");
    QAction *selected = menu.exec(senderList->viewport()->mapToGlobal(pos));
    if (selected == remove) {
        removeSelectedTask(item);
    }
}

QString MainWindow::getCategoryOfItem(QListWidgetItem * item) const {
    for (auto it = categoryLists.begin(); it != categoryLists.end(); ++it) {
        if (it.value()->findItems(item->text(), Qt::MatchExactly).contains(item)) {
            return it.key();
        }
    }
    return "";
}

void MainWindow::loadTasksToUI() {
    for (auto lsit : categoryLists.values()) {
        lsit->clear();
    }

    for (const QString &category : categoryLists.keys()) {
        QList<Task> tasks = taskHandler.getTasksByCategory(category);
        for (const Task &t : tasks) {
            QString display = QString("%1  | 🏷 %2 | ⏳ %3").arg(t.name, t.tags, t.deadline.toString("yyyy-MM-dd"));
            QListWidgetItem *item = new QListWidgetItem(display);
            item->setCheckState(t.completed ? Qt::Checked : Qt::Unchecked);
            item->setData(Qt::UserRole, t.id);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
            categoryLists[category]->addItem(item);
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
