LIBS=-lpthread
all:mutithread_epoll_server.c mutithread_epoll_client.c
	gcc mutithread_epoll_server.c -o mutithread_epoll_server ${LIBS}
	gcc mutithread_epoll_client.c -o mutithread_epoll_client ${LIBS}
clean:
	rm -f mutithread_epoll_server mutithread_epoll_client