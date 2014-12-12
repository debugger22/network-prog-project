#include "client.h"
#include "ui_client.h"

using namespace std;

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    loglist = new QStringList();
    model = new QStringListModel();
    ui->logListView->setModel(model);
    ui->fileProgressBar->setVisible(false);
}

void Client::setPortNo(int portno){
    this->portno = portno;
}

void Client::setServerIp(std::string ip){
    this->server_ip = ip;
}


Client::~Client()
{
    delete ui;
}

void Client::on_cmdSendFile_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    //dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
        filepath = fileNames.at(0).toStdString();
        //filename = filepath.allocator_type
        std::cout << fileNames.at(0).toStdString() << std::endl;

        this->filename = filepath.substr(filepath.find_last_of("/") + 1);

        thread = new QThread;
        worker = new ClientWorker;
        worker->parent = this;
        worker->portno = this->portno;
        worker->ip = this->server_ip;
        worker->type = 0;  // A file has to be sent
        worker->filename = filepath;
        worker->filesimplename = this->filename;
        worker->moveToThread(thread);
        thread->start();
        QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
    }
    else{
        std::cout << "no file was selected" << std::endl;
    }
}

void Client::on_cmdSendMessage_clicked()
{
    std::string message = ui->txtMessage->text().toStdString();
    std::cout<< message << std::endl;
    thread = new QThread;
    worker = new ClientWorker;
    worker->parent = this;
    worker->portno = this->portno;
    worker->ip = this->server_ip;
    worker->type = 1;  // A message has to be sent
    worker->message = message;
    worker->moveToThread(thread);
    thread->start();
    QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
}

void Client::fileUploadProgressChanged(int progress){
    if(progress == 100){
        ui->fileProgressBar->setVisible(false);
    }
    if(progress > 0 && progress < 100){
        ui->fileProgressBar->setVisible(true);
    }
    ui->fileProgressBar->setValue(progress);
}

void Client::fileUploadedSuccessfully(std::string filename){
    cout << "File uploaded successfully!(Main thread) "<< filename << endl;
    loglist->append(QString::fromUtf8(filename.c_str()) +"......... Sent");
    model->setStringList(*loglist);
}

void Client::messageSentSuccessfully(std::string message){
    loglist->append(QString::fromUtf8(message.c_str()));
    model->setStringList(*loglist);
}

void Client::fileQueued(std::string filename){
    loglist->append("Queued..........." + QString::fromUtf8(filename.c_str()));
    model->setStringList(*loglist);
}

void Client::intChanged(){
    loglist->append(QString::fromUtf8("Interface changed to LAN"));
    model->setStringList(*loglist);
    sleep(2);
}
