#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

namespace Ui {
class HelpWindow;
}

class HelpWindow : public QDialog
{
    //required by QDialog to connect signals and slots
    Q_OBJECT
    
public:

    // constructor for the HelpWindow
    /*
     * *parent = parent widget for the dialog
     *
     *searches in the current directory of the program and attempts to open a file "Help.txt"
     *if the file is not found the display will notify the user the help file could not be found
     *
     */
    explicit HelpWindow(QWidget *parent = 0);
    ~HelpWindow();
    
private:

    // creates the user interface and provides a pointer to the interface
    Ui::HelpWindow *ui;
};

#endif // HELPWINDOW_H
