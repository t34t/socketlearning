LIBS=-lssl -lcrypto
all:server1.c client1.c
	gcc server1.c -o socketserver1 ${LIBS}
	gcc client1.c -o socketclient1 ${LIBS}

       #如果是自定义安装路径的，可以使用下面的
       #这个有点问题，以上面的为准
#gcc -Wall -o clientclient.c -I/usr/openssl-1.0.0c/include/usr/openssl-1.0.0c/libssl.a /usr/openssl-1.0.0c/libcrypto.a –ldl
#gcc -Wall -o serverserver.c -I/usr/openssl-1.0.0c/include/usr/openssl-1.0.0c/libssl.a /usr/openssl-1.0.0c/libcrypto.a -ldl
clean::
	rm -f socketserver socketclient
