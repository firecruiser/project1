#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "turret.h"
#include <QtGui>
#include <QtCore>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/MainWindowIcon.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

