#include <iostream>
#include <sstream>
#include <string>
#include <QThread>

#include "server.h"
#include "ui_server.h"

namespace patch{
    template < typename T > std::string to_string( const T& n ){
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace std;

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server){
    ui->setupUi(this);
    loglist = new QStringList();
    model = new QStringListModel();
    ui->logListView->setModel(model);
    ui->fileProgressBar->setVisible(false);
}

Server::~Server()
{
    delete ui;
}

void Server::setPortNo(int portno){
    this->portno = portno;
    this->setWindowTitle(QString::fromUtf8(("Server at port "
                                            + patch::to_string(this->portno)).c_str()));
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
    //cout << "Client connected successfully with IP: " << ip << endl;
    //ui->lblClientIP->setText(QString::fromUtf8(ip.c_str()));
}

void Server::fileDownloadProgressChanged(int progress){
    if(progress == 100){
        ui->fileProgressBar->setVisible(false);
    }
    if(progress > 0 && progress < 100){
        ui->fileProgressBar->setVisible(true);
    }
    ui->fileProgressBar->setValue(progress);
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
