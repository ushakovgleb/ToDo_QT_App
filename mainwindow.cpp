#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskdatabase.h"

#include <QVBoxLayout>

TaskDatabase db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ToDo App");
    resize(1000, 1000);

    ui->deadlineEdit->setDate(QDate::currentDate());

    categoryLists["Сегодня"] = ui->listToday;
    categoryLists["Завтра"] = ui->listTomorrow;
    categoryLists["След. неделя"] = ui->listNextWeek;
    categoryLists["Потом"] = ui->listLater;

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addTask);

    // Цикл по всем спискам задач
    for (auto list : categoryLists) {

        // Контекстное меню
        list->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(list, &ListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

        connect(list, &ListWidget::itemChanged, this, [this](QListWidgetItem *item) {

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
            tsk.deadline = item->text().section("|", 2, 2).replace("⏳", "").trimmed(), "yyyy-MM-dd"); // Дата
            tsk.category = getCategoryOfItem(item); // Категория
            tsk.done = item->checkState() == Qt::Checked; // Статус выполнения задачи


            taskManager.updateTask(t);

        });

    }

    loadTaskToUI();
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
    task.deadline = date;
    task.category = category;
    task.done = false;

    int id = taskManager.addTask(task);

    QString display = QString("%1  | 🏷 %2 | ⏳ %3").arg(name, tags, date.toString("yyyy-MM-dd"));
    QListWidgetItem * item = new QListWidgetItem(display);
    item->setCheckState(Qt::Unchecked);
    item->setData(Qt::UserRole, id);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEditable);
    categoryLists[category]->addItem(item);

    ui->taskInput->clear();
    ui-tagInput->clear();
    ui->deadlineEdit->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}
