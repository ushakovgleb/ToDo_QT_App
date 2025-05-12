#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ToDo App");
    resize(1000, 1000);

    QWidget *central = new QWidget;
    // QVBoxLayout *mainLayout = new QVBoxLayout(central);
    central->setStyleSheet("background-color: #1e1e1e; color: white");

}

MainWindow::~MainWindow()
{
    delete ui;
}
