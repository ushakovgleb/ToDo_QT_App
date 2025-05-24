#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskdatabase.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QMenu>
#include <QDate>
#include <QInputDialog>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

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

void MainWindow::removeAllTasks(QListWidget *list) {
    if (!list) return;

    for (int i = list->count() - 1; i >= 0; --i) {
        QListWidgetItem *item = list->item(i);
        int id = item->data(Qt::UserRole).toInt();
        taskHandler.deleteTask(id);
        delete item;
    }
}

void MainWindow::removeSelectedItems(QListWidget *list) {
    if (!list) return;

    for (int i = list->count() - 1; i >= 0; --i) {
        QListWidgetItem *item = list->item(i);
        if (item->checkState() == Qt::Checked) {
            int id = item->data(Qt::UserRole).toInt();
            taskHandler.deleteTask(id);
            delete item;
        }
    }
}

void MainWindow::editTask(QListWidgetItem *item) {
    if (!item) return;

    QString oldName = item->text().section("|", 0, 0).trimmed();
    QString oldTags = item->text().section("|", 1, 1).replace("🏷", "").trimmed();
    QDate oldDate = QDate::fromString(item->text().section("|", 2, 2).replace("⏳", "").trimmed(), "yyyy-MM-dd");
    QString oldCategory = getCategoryOfItem(item);

    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать задачу");

    QFormLayout form(&dialog);

    QLineEdit *nameEdit = new QLineEdit(oldName, &dialog);
    QLineEdit *tagEdit = new QLineEdit(oldTags, &dialog);
    QDateEdit *dateEdit = new QDateEdit(oldDate, &dialog);
    dateEdit->setCalendarPopup(true);
    QComboBox *categoryBox = new QComboBox(&dialog);
    categoryBox->addItems(categoryLists.keys());
    categoryBox->setCurrentText(oldCategory);

    form.addRow("Имя задачи:", nameEdit);
    form.addRow("Теги:", tagEdit);
    form.addRow("Дедлайн:", dateEdit);
    form.addRow("Категория:", categoryBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);

    form.addRow(&buttonBox);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = nameEdit->text().trimmed();
        QString newTags = tagEdit->text().trimmed();
        QDate newDate = dateEdit->date();
        QString newCategory = categoryBox->currentText();

        if (!newName.isEmpty()) {
            QString updatedText = QString("%1 | 🏷 %2 | ⏳ %3").arg(newName, newTags, newDate.toString("yyyy-MM-dd"));
            item->setText(updatedText);

            int id = item->data(Qt::UserRole).toInt();
            item->setData(Qt::UserRole, id);
            item->setCheckState(item->checkState());

            if (newCategory != oldCategory) {
                QListWidget *oldList = categoryLists[oldCategory];
                QListWidget *newList = categoryLists[newCategory];
                oldList->takeItem(oldList->row(item));
                newList->addItem(item);
            }

            Task updatedTask;
            updatedTask.id = id;
            updatedTask.name = newName;
            updatedTask.tags = newTags;
            updatedTask.deadline = newDate;
            updatedTask.category = newCategory;
            updatedTask.completed = item->checkState() == Qt::Checked;

            taskHandler.updateTask(updatedTask);
        }
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QListWidget *senderList = qobject_cast<QListWidget*>(sender());
    if (!senderList) return;
    QListWidgetItem * item = senderList->itemAt(pos);
    if (!item) return;

    QMenu menu;
    QAction *editAction = menu.addAction("Редактировать");
    QAction *removeAction = menu.addAction("Удалить");
    QAction *removeAllAction = menu.addAction("Очистить список");
    QAction *removeSelectedAction = menu.addAction("Удалить выполненные задачи");

    QAction *selected = menu.exec(senderList->viewport()->mapToGlobal(pos));

    if (selected == editAction) {
        editTask(item);
    } else if (selected == removeAction) {
        removeSelectedTask(item);
    } else if (selected == removeAllAction) {
        removeAllTasks(senderList);
    } else if (selected == removeSelectedAction) {
        removeSelectedItems(senderList);
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
