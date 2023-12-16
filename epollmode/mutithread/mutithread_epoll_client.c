#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc,const char * argv[])
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1)
    {
        perror("socekt error");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr.s_addr);

    int ret = connect(fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(ret == -1)
    {
        perror("connect error");
        exit(1);
    }

    int num = 0;
    char buf[1024] = {0};

    while(num < 200)
    {
        sprintf(buf,"hello, world, %d\n...",num++);
        printf("%s\n",buf);

        write(fd,buf,strlen(buf)+1);

        recv(fd,buf,sizeof(buf),0);
        printf("recv msg: %s\n",buf);

        usleep(100000); 
    }

    printf("------program over-------");
    close(fd);
    
    return 0;
}
