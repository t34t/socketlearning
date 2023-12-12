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

#define MSG_STR "Hello Huyuanzhuo iot studio\n"


int main(int argc, char **argv)
{

    int                     sockfd = -1;
    struct sockaddr_in      servaddr;
    char                    *servip;
    int                     port = 0;
    int                     ch;
    int                     rv = -1;
    char                    buf[1024];

    if(argc < 3)
    {
        printf("program usage: %s[Server_ip] [Port]\n", argv[0]);
        return -1;
    }

    servip = argv[1];
    port = atoi(argv[2]);


    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        printf("Create socket failure:%s\n", strerror(errno));
        return -1;
    }
    printf("Create socket successfully: %d\n", sockfd);


    memset( &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_aton(servip, &servaddr.sin_addr);

    rv = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if(rv < 0)
    {
        printf("Connect to server [%s:%d] failed:%s\n",servip, port, strerror(errno));
        return -2;
    }

    printf("Connect to server[%s:%d] successfully\n",servip, port);


    while(1)
    {
        rv = write(sockfd, MSG_STR, strlen(MSG_STR));
        if(rv < 0)
        {
            printf("Write to server by sockfd[%d] failure: %s\n", sockfd, strerror(errno));
            break;
        }

        memset(buf, 0, sizeof(buf));
        rv = read(sockfd, buf, sizeof(buf));
        if(rv < 0)
        {
            printf("Read data from server by sockfd[%d] failure: %s\n", sockfd, strerror(errno));
            break;
        }
        else if(rv == 0)
        {
            printf("socket[%d] get disconnected\n", sockfd);
            break;
        }

        else if(rv > 0)
        {
            printf("Read %d bytes data from server: %s\n", rv, buf);
            continue;

        }


   }

   close(sockfd);

}


