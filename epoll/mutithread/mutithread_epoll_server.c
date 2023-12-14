#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

typedef struct socketinfo
{
    int fd;
    int epfd;
} SocketInfo;

void *acceptConn(void *arg)
{
    printf("acceptconn tid: %ld\n", pthread_self());

    SocketInfo *info = (SocketInfo *)arg;

    int cfd = accept(info->fd, NULL, NULL);

    // 设置非阻塞属性
    int flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = cfd;
    epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);

    free(info);
    return NULL;
}

void *communication(void *arg)
{
    printf("communication  tid: %ld\n", pthread_self());
    SocketInfo *info = (SocketInfo *)arg;
    int fd = info->fd;
    int epfd = info->epfd;

    char temp[1024];
    bzero(temp,sizeof(temp)); 

    char buf[5];
    while (1)
    {
        int len = recv(fd, buf, sizeof(buf), 0);
        if (len == -1)
        {
            if (errno == EAGAIN)
            {
                printf("数据已经接收完毕");
                send(fd,temp,strlen(temp)+1,0);
                break;
            }
            else
            {
                perror("recv error");
                //exit(1);
                break;
            }
        }
        else if (len == 0)
        {
            printf("客户端已经断开连接....\n");
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
            close(fd);
            break;
        }
        //printf("read buf = %s\n", buf);

        for (int i = 0; i < len; i++)
        {
            buf[i] = toupper(buf[i]);
            
        }
        write(STDOUT_FILENO, buf, len);
        strncat(temp+strlen(temp),buf,len);
/*
        ret = send(fd, buf, strlen(buf) + 1, 0);
        if (ret == -1)
        {
            perror("send error");
            exit(1);
        }*/
    }
    free(info);
    return NULL;
}

int main(int argc, const char *argv[])
{
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int optval = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int ret = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    ret = listen(lfd, 64);
    if (ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    int epfd = epoll_create(1);
    if (epfd == -1)
    {
        perror("epoll_create");
        exit(0);
    }

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);

    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(evs[0]);

    while (1)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        printf("num = %d\n", num);
        pthread_t tid;
        SocketInfo * info =  (SocketInfo*)malloc(sizeof(SocketInfo));

        for (int i = 0; i < num; i++)
        {
            int fd = evs[i].data.fd;

            info->fd = fd;
            info->epfd = epfd;

            if (fd == lfd)
            {
                pthread_create(&tid,NULL,acceptConn,info);
                pthread_detach(tid);
            }
            else
            {
                pthread_create(&tid,NULL,communication,info);
                pthread_detach(tid);
            }

        }
    }
    close(lfd);
    return 0;
}