#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtCore>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMessageBox::information(this,"","");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Testing
void MainWindow::on_pushButton_clicked()
{
    int r=QMessageBox::question(this,"","Exit?",QMessageBox::Yes|QMessageBox::No);
    if(r==QMessageBox::Yes)
    {
        close();
    }
}
