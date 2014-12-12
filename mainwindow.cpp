#include <iostream>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdLaunchServer_clicked()
{

    if(ui->txtServeStartPort->text().length() == 0){
        QMessageBox * box = new QMessageBox;
        box->setText("Please provide port number in order to start the server.");
        box->setWindowTitle("Port number is required");
        box->show();
        return;
    }
    Server *serverWindow = new Server;
    serverWindow->setPortNo(ui->txtServeStartPort->text().toInt());
    serverWindow->startServer();
    serverWindow->show();
    this->hide();
    std::cout << "Server started successfully!" << std::endl;
}

void MainWindow::on_cmdLaunchClient_clicked()
{
    if(ui->txtServerConnectPort->text().length() == 0 || ui->txtServerIP->text().length() == 0){
        QMessageBox * box = new QMessageBox;
        box->setText("Please provide port number and Ip of the server.");
        box->setWindowTitle("IP/Port number is required");
        box->show();
        return;
    }
    Client *clientWindow = new Client;
    clientWindow->setPortNo(ui->txtServerConnectPort->text().toInt());
    clientWindow->setServerIp(ui->txtServerIP->text().toStdString());
    clientWindow->show();
    this->hide();
    std::cout << "Client started!" << std::endl;
}
