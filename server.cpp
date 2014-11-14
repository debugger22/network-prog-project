#include <iostream>
#include <QThread>

#include "server.h"
#include "ui_server.h"

using namespace std;

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server){
    ui->setupUi(this);
    loglist = new QStringList();
    model = new QStringListModel();
    ui->logListView->setModel(model);
}

Server::~Server()
{
    delete ui;
}

void Server::setPortNo(int portno){
    this->portno = portno;
}

void Server::startServer(){

    thread = new QThread;
    worker = new ServerWorker;
    worker->parent = this;
    worker->portno = this->portno;
    worker->moveToThread(thread);
    thread->start();
    QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
    connect(this, SIGNAL(closeServer()), worker, SLOT(closeServer()), Qt::DirectConnection);

}

void Server::clientConnectedSuccessfully(std::string ip){
    cout << "Client connected successfully with IP: " << ip << endl;
    ui->lblClientIP->setText(QString::fromUtf8(ip.c_str()));
}


void Server::fileReadSuccessfully(std::string filename){
    cout << "File read successfully!(Main thread) "<< filename << endl;
    loglist->append(QString::fromUtf8(filename.c_str()));
    model->setStringList(*loglist);
}

void Server::messageReadSuccessfully(std::string message){
    cout << "Message read successfully!(Main thread) "<< message << endl;
    loglist->append(QString::fromUtf8(message.c_str()));
    model->setStringList(*loglist);
}

void Server::on_cmdCloseServer_pressed(){
    emit closeServer();
    this->close();
}
