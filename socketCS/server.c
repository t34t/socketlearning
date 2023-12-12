#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdlib.h>

#define MSG_STR "Hello Huyuanzhuo  iot studio client\n"
#define BACKLOG 13

int main(int argc, char **argv)
{
    int                 sockfd = -1;
    int                 clifd;
    struct sockaddr_in  serv_addr;
    struct sockaddr_in  cli_addr;
    socklen_t           cliaddr_len;
    int                 port = 0;
    int                 ch;
    int                 rv = -1;
    int                 on = 1;
    char                buf[1024];

    if(argc <2)
    {
        printf("program usage: %s [Serv_port]\n", argv[0]);
        return -1;
    }
    port = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0 )
    {
        printf("Create socket failure:%s\n",strerror(errno));
        return -1;
    }
    printf("Create socket successfully: %d\n", sockfd);

    //  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(servaddr));

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port  = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    rv = bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(rv < 0)
    {
        printf("socket[%d] bind to the port[%d] failure:%s\n",sockfd, port,strerror(errno));
        return -2;
    }


    listen(sockfd,BACKLOG);
    printf("Start to listen on port [%d]\n", port);
    while(1)
    {
        printf("Start to wait and accept new client...\n");
          clifd = accept(sockfd, (struct sockaddr *)&cli_addr, &cliaddr_len);
        if(clifd < 0)
        {
           printf("Accept client failure:%s\n",strerror(errno));
           continue;
        }

        printf("Accept new client[%s:%d] successfully\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        memset(buf, 0, sizeof(buf));



        rv=read(clifd, buf, sizeof(buf));
        if(rv < 0)
        {
            printf("Read data from client sockfd[%d] failure:%s\n", clifd,strerror(errno));
            close(clifd);
            continue;
        }

        else if(rv == 0)
        {
            printf("socket[%d] get disconnected\n", clifd);
            close(clifd);
            continue;
        }


        else if(rv > 0)
        {
            printf("Read %d data from server:%s\n", rv, buf);
        }


        rv = write(clifd, MSG_STR,strlen(MSG_STR));
        if(rv < 0)
        {
            printf("Write to client by sockfd[%d] failure: %s\n", sockfd, strerror(errno));
            close(clifd);
            continue;
        }

        printf("Close client socket[%d]\n", clifd);

    }
     close(sockfd);
   return 0;
}

