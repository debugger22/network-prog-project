#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
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

class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = 0);
    QObject *parent;
    int sockfd, portno, n, clilen, newsockfd;
    char buffer[256];
    char messageBuffer[10*1024];
    static const int BUFFER_SIZE = 5*1024*1024;
    char filebuffer[BUFFER_SIZE];

    FILE *fp;
    struct sockaddr_in serversock_addr;
    struct sockaddr_in clisock_addr;

signals:
    void clientConnectedSuccessfullySignal(std::string ip);
    void fileDownloadProgressChangedSignal(int progress);
    void fileReadSuccessfullySignal(std::string filename);
    void messageReadSuccessfullySignal(std::string message);

public slots:
    void doWork();
    void closeServer();

    
};

#endif // SERVERWORKER_H
