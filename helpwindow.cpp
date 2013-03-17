#include "helpwindow.h"
#include "ui_helpwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>


// constructor that reads in the Help.txt file
HelpWindow::HelpWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    QString filename=QApplication::applicationDirPath()+"/Help.txt";
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        ui->helpView->setText("Unable to find Help.txt! Please ensure Help.txt is located in the application's directory");
    }
    else
    {
        QString helpdata;
            QTextStream in(&file);
            helpdata=in.readAll();
            ui->helpView->setText(helpdata);
    }

}

// window destructor
HelpWindow::~HelpWindow()
{
    delete ui;
}
