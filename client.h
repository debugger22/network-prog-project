#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <QDialog>
#include <QFileDialog>
#include <QThread>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>

#include "clientworker.h"

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT
    
public:
    explicit Client(QWidget *parent = 0);
    std::string filename;
    std::string filepath;
    std::string server_ip;
    int portno;
    void setPortNo(int portno);
    void setServerIp(std::string ip);
    QThread *thread;
    ClientWorker *worker;
    QStringList *loglist;
    QStringListModel *model;
    ~Client();
    
private slots:
    void on_cmdSendFile_clicked();

    void on_cmdSendMessage_clicked();

public slots:
    //void serverConnectedSuccessfully(std::string ip);
    void fileUploadProgressChanged(int progress);
    void fileUploadedSuccessfully(std::string filename);
    void messageSentSuccessfully(std::string message);
    void fileQueued(std::string filename);
    void intChanged();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
