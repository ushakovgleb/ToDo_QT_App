#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "taskhandler.h"

#include <QMainWindow>
#include <QListWidget>
#include <QMap>



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTask();
    void removeSelectedTask(QListWidgetItem *item);
    void showContextMenu(const QPoint &pos);
    void loadTasksToUI();

private:
    Ui::MainWindow *ui;
    QMap<QString, QListWidget*> categoryLists;
    QString getCategoryOfItem(QListWidgetItem * item) const;
    TaskHandler taskHandler;

};
#endif // MAINWINDOW_H
