#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "turret.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "readimages.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


signals:
    void stopVideo();
protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_upbutton_pressed();

    void on_upbutton_released();

    void on_rightbutton_pressed();

    void on_rightbutton_released();

    void on_leftbutton_pressed();

    void on_leftbutton_released();

    void on_downbutton_pressed();

    void on_downbutton_released();

    void on_firebutton_clicked();

    void on_reloadbutton_clicked();

    void on_startstopbutton_clicked();

    void on_actionReload_triggered();

    void on_actionCalibrate_triggered();

    void on_calibratebutton_clicked();

    void on_actionConnect_to_Turret_triggered();

    void on_actionConnect_to_Camera_triggered();

    void updatepic(QPixmap);

    void errorstring(QString);
private:
    Ui::MainWindow *ui;
    Turret *turr;
    QThread* thread;
    ReadImages* readimg;
    void senderror();
    void enablebuttons();
    void disablebuttons();
};

#endif // MAINWINDOW_H
