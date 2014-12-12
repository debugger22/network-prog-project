#ifndef CLIENTWORKER_H
#define CLIENTWORKER_H

#include <QObject>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/stat.h>


class ClientWorker : public QObject{

    Q_OBJECT
    public:
        explicit ClientWorker(QObject *parent = 0);
        int type;
        QObject *parent;
        std::string ip;
        int sockfd, portno, n, clilen, newsockfd;
        char buffer[256];
        static const int BUFFER_SIZE = 16*1024;
        char messageBuffer[BUFFER_SIZE];
        char filebuffer[BUFFER_SIZE];

        FILE *fp;
        struct sockaddr_in servsock_addr;
        struct hostent *server;
        std::string filename;
        std::string message;
        std::string filesimplename;

    signals:
        void serverConnectedSuccessfullySignal(std::string ip);
        void fileUploadProgressChangedSignal(int progress);
        void fileUploadedSuccessfullySignal(std::string filename);
        void fileQueuedSignal(std::string filename);
        void messageSentSuccessfullySignal(std::string message);
        void intChangedSignal();

    public slots:
        void doWork();
        //void closeClient();
};

#endif // CLIENTWORKER_H
