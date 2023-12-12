## network select mode

### 基本概念
select的大概思想：将多个套接字放在一个集合里，然后统一检查这些套接字的状态（可读、可写、异常等），调用select后，会更新这些套接字的状态，然后做判断，如果套接字可读，就执行read操作。这样就巧妙地避免了阻塞，达到同时处理多个连接的目的。当然如果没有事件发生，select会一直阻塞，如果不想一直让它等待，想去处理其它事情，可以设置一个最大的等待时间。

select模型解决基本C/S模型中，accept()、recv()、send()阻塞的问题，以及C/S模型需要创建大量线程，客户端过多就会增加服务器运行压力

```
int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout); 
```