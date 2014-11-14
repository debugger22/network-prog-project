#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_cmdLaunchServer_pressed();  // Launch server button click slot

    void on_cmdLaunchClient_pressed();  // Launch client button click slot

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
