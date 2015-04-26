#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>  // for getting file size using stat()
#include <sys/sendfile.h>  // for sendfile()
#include <fcntl.h>  // for O_RDONLY

int main(int argc,char *argv[]){
    struct sockaddr_in server;
    struct stat obj;
    int sock;
    int choice;
    char buf[100], command[5], filename[20], *f, filenames[100], filenames_put[100];
    int k, size, status;
    int filehandle;
    
    int no_of_files = 1;
    int z = 0, y = 0;
    int temp;  
    
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1){
      printf("Couldn't create socket");
      exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = atoi(argv[1]);
    server.sin_addr.s_addr = 0;
    k = connect(sock,(struct sockaddr*)&server, sizeof(server));
    if(k == -1){
        printf("Connect Error");
        exit(1);
    }
    int i = 1;
    printf("connected");
    while(1){
        printf("Enter a choice:\n1- get\n2- put\n3- mget\n4- mput\n5- del\n6- ls\n7- quit\n");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                printf("Enter the filename: ");
                scanf("%s", filename);
                strcpy(buf, "get ");
                strcat(buf, filename);
                send(sock, buf, 100, 0);
                recv(sock, &size, sizeof(int), 0);
                if(!size){
                    printf("File not found\n\n");
                    break;
                }
                f = malloc(size);
                recv(sock, f, size, 0);
                while(1){
                    filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
                    if(filehandle == -1){
                        // Needed only if same directory is used for both server and client
                        sprintf(filename + strlen(filename), "%d", i);
                    }
                    else break;
                }
                write(filehandle, f, size, 0);
                close(filehandle);
                strcpy(buf, "cat ");
                strcat(buf, filename);
                system(buf);
                break;
            case 2:
                printf("Enter the filename: ");
                scanf("%s", filename);
                filehandle = open(filename, O_RDONLY);
                if(filehandle == -1){
                    printf("File not found\n\n");
                    break;
                }
                strcpy(buf, "put ");
                strcat(buf, filename);
                send(sock, buf, 100, 0);
                stat(filename, &obj);
                size = obj.st_size;
                send(sock, &size, sizeof(int), 0);
                sendfile(sock, filehandle, NULL, size);
                recv(sock, &status, sizeof(int), 0);
                if(status) printf("File saved successfully!\n");
                else printf("Failed to save file on the server\n");
                break;
            case 3:
                //bzero(&filenames, sizeof(filenames));
                no_of_files = 1, y = 0, z = 0;
                scanf("%d", &temp);
                fgets(filenames, 100, stdin);
                temp = 0;
                while(1){
                    if(filenames[z] == ' '){
                        //no_of_files++;
                        for(y = 0; y <= z - temp; y++){
                            filename[y] = filenames[temp + y];
                        }
                        //filename[y + 1] = '\0';
                        printf("going to get %s\n", filename);
                        /* ------ Get File -----0-*/
                        strcpy(buf, "get ");
                        strcat(buf, filename);
                        send(sock, buf, 100, 0);
                        recv(sock, &size, sizeof(int), 0);
                        if(!size){
                            printf("File not found\n\n");
                            break;
                        }
                        f = malloc(size);
                        recv(sock, f, size, 0);
                        while(1){
                            filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
                            if(filehandle == -1){
                                // Needed only if same directory is used for both server and client
                                sprintf(filename + strlen(filename), "%d", i);
                            }
                            else break;
                        }
                        write(filehandle, f, size, 0);
                        close(filehandle);
                        /* --------*/

                        temp = z + 1;
                    }else if(filenames[z] == '\0' || filenames[z] == '\n'){
                        for(y = 0; y <= z - temp; y++){
                            filename[y] = filenames[temp + y];
                        }
                        //filename[y + 1] = '\0';
                        printf("going to get %s\n", filename);
                        /* ------ Get File -----0-*/
                        strcpy(buf, "get ");
                        strcat(buf, filename);
                        send(sock, buf, 100, 0);
                        recv(sock, &size, sizeof(int), 0);
                        if(!size){
                            printf("File not found\n\n");
                            break;
                        }
                        f = malloc(size);
                        recv(sock, f, size, 0);
                        while(1){
                            filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
                            if(filehandle == -1){
                                // Needed only if same directory is used for both server and client
                                sprintf(filename + strlen(filename), "%d", i);
                            }
                            else break;
                        }
                        write(filehandle, f, size, 0);
                        close(filehandle);
                        /* --------*/

                        break;
                    }
                    z++;
                }
                break;
            case 4:
                no_of_files = 1, y = 0, z = 0;
                scanf("%d", &temp);
                fgets(filenames_put, 100, stdin);
                temp = 0;
                while(1){
                    if(filenames_put[z] == ' '){
                        //no_of_files++;
                        for(y = 0; y <= z - temp; y++){
                            filename[y] = filenames_put[temp + y];
                        }
                        filename[y + 1] = '\0';
                        printf("going to send %s\n", filename);
                        
                        /*SEND FILE*/
                        filehandle = open(filename, O_RDONLY);
                        if(filehandle == -1){
                            printf("File not found\n\n");
                            break;
                        }
                        strcpy(buf, "put ");
                        strcat(buf, filename);
                        send(sock, buf, 100, 0);
                        stat(filename, &obj);
                        size = obj.st_size;
                        send(sock, &size, sizeof(int), 0);
                        sendfile(sock, filehandle, NULL, size);
                        recv(sock, &status, sizeof(int), 0);
                        //if(status) printf("File saved successfully!\n");
                        //else printf("Failed to save file on the server\n");
                        
                        /*---*/
                    }else if(filenames_put[z] == '\0' || filenames_put[z] == '\n'){
                        for(y = 0; y <= z - temp; y++){
                            filename[y] = filenames_put[temp + y];
                        }
                        filename[y + 1] = '\0';
                        printf("going to send %s\n", filename);

                        /*SEND FILE*/
                        filehandle = open(filename, O_RDONLY);
                        if(filehandle == -1){
                            printf("File not found\n\n");
                            break;
                        }
                        strcpy(buf, "put ");
                        strcat(buf, filename);
                        send(sock, buf, 100, 0);
                        stat(filename, &obj);
                        size = obj.st_size;
                        send(sock, &size, sizeof(int), 0);
                        sendfile(sock, filehandle, NULL, size);
                        recv(sock, &status, sizeof(int), 0);
                        //if(status) printf("File saved successfully!\n");
                        //else printf("Failed to save file on the server\n");
                        
                        /*---*/
                        break;
                    }
                    z++;
                }
                break;
            case 5:
                printf("Enter the filename: ");
                scanf("%s", filename);
                filehandle = open(filename, O_RDONLY);
                if(filehandle == -1){
                    printf("File not found\n\n");
                    break;
                }
                strcpy(buf, "del ");
                strcat(buf, filename);
                send(sock, buf, 100, 0);
                stat(filename, &obj);
                size = obj.st_size;
                send(sock, &size, sizeof(int), 0);
                sendfile(sock, filehandle, NULL, size);
                recv(sock, &status, sizeof(int), 0);
                if(status) printf("File removed successfully!\n");
                else printf("Failed to remove file on the server\n");
                break;
            case 6:
                strcpy(buf, "ls");
                send(sock, buf, 100, 0);
                recv(sock, &size, sizeof(int), 0);
                f = malloc(size);
                recv(sock, f, size, 0);
                filehandle = creat("temp1.txt", O_RDWR);
                write(filehandle, f, size, 0);
                close(filehandle);
                printf("Remote directory contents:\n");
                system("cat temp1.txt");
                break;
            case 7:
                strcpy(buf, "quit");
                send(sock, buf, 100, 0);
                recv(sock, &status, 100, 0);
                if(status){
                    printf("Server closed\nQuitting..\n");
                    exit(0);
                }
                printf("Server failed to close connection\n");
        }
    }
}
