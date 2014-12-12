#include <iostream>
#include <string>
#include <sstream>
#include "serverworker.h"

namespace patch{
    template < typename T > std::string to_string( const T& n ){
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace std;

ServerWorker::ServerWorker(QObject *parent) :
    QObject(parent){
}

void ServerWorker::doWork(){
    connect(this, SIGNAL(fileDownloadProgressChangedSignal(int)),
            parent, SLOT(fileDownloadProgressChanged(int)));
    connect(this, SIGNAL(fileReadSuccessfullySignal(std::string)),
            parent, SLOT(fileReadSuccessfully(std::string)), Qt::DirectConnection);
    connect(this, SIGNAL(messageReadSuccessfullySignal(std::string)),
            parent, SLOT(messageReadSuccessfully(std::string)), Qt::DirectConnection);
    connect(this, SIGNAL(clientConnectedSuccessfullySignal(std::string)),
            parent, SLOT(clientConnectedSuccessfully(std::string)), Qt::DirectConnection);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
            cout << "Couldn't create socket" << endl;
            exit(1);
    }

    bzero(&serversock_addr, sizeof(serversock_addr));
    serversock_addr.sin_port = htons(portno);
    serversock_addr.sin_family = AF_INET;
    serversock_addr.sin_addr.s_addr = INADDR_ANY;
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    if(bind(sockfd, (struct sockaddr *)&serversock_addr, sizeof(serversock_addr))<0){
            cout << "Couldn't bind to the socket" << endl;
            exit(1);
    }
    listen(sockfd, 5);
    bzero(&clisock_addr, sizeof(clisock_addr));
    clilen = sizeof(clisock_addr);

    while(1){
        int filelength = 0;
        int size = 0;
        newsockfd = accept(sockfd, (struct sockaddr *)&clisock_addr, (socklen_t *)&clilen);
        std::string client_ip = "";
        client_ip += patch::to_string((int)(clisock_addr.sin_addr.s_addr&0xFF)) + ".";
        client_ip += patch::to_string((int)((clisock_addr.sin_addr.s_addr&0xFF)>>8)) + ".";
        client_ip += patch::to_string((int)((clisock_addr.sin_addr.s_addr&0xFF0000)>>16)) + ".";
        client_ip += patch::to_string((int)((clisock_addr.sin_addr.s_addr&0xFF000000)>>24));
        emit clientConnectedSuccessfullySignal(client_ip);
        if(newsockfd >= 0){
            int data_type;
            read(newsockfd, &data_type, sizeof(data_type));
            if(data_type==0){
                read(newsockfd,&filelength, sizeof(filelength));
                cout << filelength << endl;
                char filename[filelength];
                char fileN[filelength+1];
                read(newsockfd,filename,filelength);
                read(newsockfd,&size,sizeof(size));
                int ch = 0;
                for(ch=0; ch < filelength; ch++){
                    fileN[ch] = filename[ch];
                }
                fileN[ch]='\0';
                emit fileReadSuccessfullySignal("Downloading " + std::string(fileN));
                filelength = strlen(fileN);
                FILE * fpIn = fopen(fileN, "w");
                int progress = 0;
                if (fpIn){
                char buf[BUFFER_SIZE];
                    while(1){
                       ssize_t bytesReceived = recv(newsockfd, buf, sizeof(buf), 0);
                       if(bytesReceived == 2896){
                           //cout << "before ending" << endl;
                       }

                       if (bytesReceived == 0) break;   // sender closed connection, must be end of file
                       progress = progress + (int)bytesReceived;
                       float per = (float)progress/size;
                       per = per * 100;
                       emit fileDownloadProgressChangedSignal((int)per);
                       //cout << "Progress " << per << endl;
                       if (bytesReceived < 0){
                           cout << "Interface changed" << endl;
                           perror("recv");  // network error?
                       }

                       if (fwrite(buf, 1, bytesReceived, fpIn) != (size_t) bytesReceived){
                          perror("fwrite");
                          break;
                       }
                    }
                }
                emit fileReadSuccessfullySignal(std::string(fileN) + " was downloaded successfully!");
                emit fileDownloadProgressChangedSignal(100);
                fclose(fpIn);
            }else if(data_type == 1){
                read(newsockfd, messageBuffer, sizeof(messageBuffer));
                emit messageReadSuccessfullySignal(messageBuffer);
                data_type = -1;
            }
            close(newsockfd);
        }
    }
}

void ServerWorker::closeServer(){
    close(newsockfd);
    close(sockfd);
    cout << "Socket server closed successfully." << endl;
}
