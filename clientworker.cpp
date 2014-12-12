#include "clientworker.h"

using namespace std;

ClientWorker::ClientWorker(QObject *parent) :
    QObject(parent){
}


void ClientWorker::doWork(){

    connect(this, SIGNAL(fileUploadProgressChangedSignal(int)),
            parent, SLOT(fileUploadProgressChanged(int)));

    connect(this, SIGNAL(fileUploadedSuccessfullySignal(std::string)),
            parent, SLOT(fileUploadedSuccessfully(std::string)), Qt::DirectConnection);

    connect(this, SIGNAL(messageSentSuccessfullySignal(std::string)),
            parent, SLOT(messageSentSuccessfully(std::string)), Qt::DirectConnection);

    connect(this, SIGNAL(fileQueuedSignal(std::string)),
            parent, SLOT(fileQueued(std::string)), Qt::DirectConnection);

    connect(this, SIGNAL(intChangedSignal()),
            parent, SLOT(intChanged()));

    emit fileQueuedSignal(this->filesimplename);
    std::cout << this->ip << std::endl;
    server = gethostbyname(this->ip.c_str());
    std::cout << h_errno << endl;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("Couldn't create socket");
        //exit(1);
    }
    bzero(&servsock_addr, sizeof(servsock_addr));
    bcopy((char *)server->h_addr, (char *)&servsock_addr.sin_addr.s_addr, server->h_length);

    servsock_addr.sin_port = htons(portno);
    servsock_addr.sin_family = AF_INET;
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "wlan0");
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr));

    if(ret < 0){
        //int perr = errno;
        printf("errorfirst: %s", strerror(ret));
        // printf("error");
    }

    if(::connect(sockfd, (struct sockaddr *)&servsock_addr, sizeof(servsock_addr)) < 0){
            printf("Couldn't connect");
            //exit(1);
    }
    const char * fileName = this->filename.c_str();

    //char filebuffer[100*1024];
    if(this->type == 0){
        FILE *fp;
        fp = fopen(fileName, "r");
        if(fp < 0){
            printf("Couldn't open");
            exit(1);
        }
        fseek(fp, 0, SEEK_END); // seek to end of file
        int size = ftell(fp); // get current file pointer
        fseek(fp, 0, SEEK_SET);

        int check;
        int c =0;
        int progress =0;
        int filelength= strlen(this->filesimplename.c_str());
        char buf[BUFFER_SIZE];
        int k = 0;
        write(sockfd,&k,sizeof(k));
        int fw = write(sockfd,&filelength,sizeof(filelength));
        int fwr = write(sockfd,this->filesimplename.c_str(),filelength);
        int fwrr = write(sockfd,&size,sizeof(size));
        //printf("%s",fileName);
        while(1){
            ssize_t bytesRead = fread(buf, 1, sizeof(buf), fp);
            if (bytesRead <= 0) break;  // EOF
            //printf("Read %i bytes from file, sending them to network...\n", (int)bytesRead);
            progress = progress + (int)bytesRead;
            if(progress == 50331648){ // 100663296
                struct ifreq ifr1;
                emit intChangedSignal();
                printf("100 MB limit exceeded switching to LAN");
                memset(&ifr1, 0, sizeof(ifr1));
                snprintf(ifr1.ifr_name, sizeof(ifr1.ifr_name), "eth0");
                int ret1 = setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr1, sizeof(ifr1));
                if(ret1 < 0){
                    //int perr = errno;
                    //printf("errorsecond: %s", strerror(ret1));
                    // printf("error");
                }
            }
            //printf("%d Progress",progress);
            float per = (float)progress/size;
            per = per * 100;
            emit fileUploadProgressChangedSignal(per);
            //printf("Progress= %f percent \n",per);
            if (send(sockfd, buf, bytesRead, 0) != bytesRead){
                perror("send");
                break;
            }
        }
        fclose(fp);
        emit fileUploadedSuccessfullySignal(this->filesimplename);
    }
    else{
        char buf[BUFFER_SIZE];
        strcpy(buf, this->message.c_str());
        int k = 1;
        write(sockfd,&k,sizeof(k));
        if(write(sockfd,buf,sizeof(buf)) >= 0)
            emit messageSentSuccessfullySignal(message);
    }
    close(sockfd);
}
