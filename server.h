#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>

#include "serverworker.h"

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT
    
public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    void setPortNo(int portno);
    void startServer();
    QThread *thread;
    ServerWorker *worker;
    QStringList *loglist;
    QStringListModel *model;

signals:
    void closeServer();
    
private:
    Ui::Server *ui;
    int portno;

 public slots:
    void clientConnectedSuccessfully(std::string ip);
    void fileDownloadProgressChanged(int progress);
    void fileReadSuccessfully(std::string filename);
    void messageReadSuccessfully(std::string message);

private slots:
    void on_cmdCloseServer_pressed();
};

#endif // SERVER_H
