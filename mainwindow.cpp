#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vector.h"
#include <QString>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Vector<int> v;


void MainWindow::on_AddButton_2_clicked()
{
    QString str = ui -> lineEdit -> text();
    int value = str.toInt();

    v.push_back(value);

    qDebug() << "Size: " << v.size() << " Capacity: " << v.capacity();

    ui -> lineEdit -> clear();
}


void MainWindow::on_ShowButton_clicked()
{
    QString result = "";
    for (auto it = v.begin(); it != v.end(); ++it) {
        qDebug() << *it;
        result = result + " " + QString::number(*it);

    }
    ui->ShowlineEdit->setText(result);
    ui->ShowlineEdit_2->setText("Size: " + QString::number(v.size()) + " Capacity: " + QString::number(v.capacity()));
}


void MainWindow::on_ShowButton_2_clicked()
{
    v.pop_back();
}


void MainWindow::on_ShowButton_3_clicked()
{
    Vector<int> v1;
    QString str = ui -> ShowlineEdit_3 -> text(); // получение текста из QLineEdit
    int value = str.toInt(); // преобразование QString в int
    int arr[] = { 1, 2, 3, 4, 5 };
    v.assign(arr, arr + 5);
    ui -> ShowlineEdit_3 -> clear();
}


void MainWindow::on_ShowButton_4_clicked()
{
    QString str = ui -> ShowlineEdit_4 -> text(); // получение текста из QLineEdit
    int value = str.toInt(); // преобразование QString в int
    v.reserve(value);
    ui -> ShowlineEdit_4 -> clear();
}

