#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


int main ()
{
	int server_sock_tcp, sock;				//дискрипторы сокетов
	struct sockaddr_in addr;				//структура с адресом
	char buf[2048];       					// буфур для приема
    int bytes_read;           				// кол-во принятых байт
    time_t utime;                      		 // Текущее время в микросекундах
    struct tm *tmp;                  	 	  // Абсолютное текущее время
    char msg[256];							// Получаемое сообщение
    pthread_t tid;                          // 

	server_sock_tcp = socket(AF_INET, SOCK_STREAM, 0);	//создаем сокет для входных подключений
	if(server_sock_tcp < 0)
    {
        perror("socket");
        exit(1);
    }
    //задаем параметры сервера
    addr.sin_family = AF_INET;
 	addr.sin_port = htons(3428);
    //addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(bind(server_sock_tcp, (struct sockaddr *)&addr, sizeof(addr)) < 0) // связываемся с сетевым устройство. 
    //Сейчас это устройство lo - "петля", которое используется для отладки сетевых приложений
    {
        perror("bind");
        exit(2);
    }
    listen(server_sock_tcp, 1); // очередь входных подключений
    sock = accept(server_sock_tcp, NULL, NULL); // принимаем входные подключение и создаем отделный сокет для каждого нового подключившегося клиента
    if(sock < 0)
    {
    	perror("Прием входящих подключений");
    	exit(3);
    }
    pthread_create(&tid, NULL, (void *)sock, (void *)sock);
        		while( 1 )
    {
    	bytes_read = recv(sock, buf, 2048, 0); // принимаем сообщение от клиента
            if(bytes_read == 0)
            	break;
    	printf("Получено %d bytes\tСообщение: %s\n", bytes_read, buf);
    	// Формируем время в нужном формате и отправляем
        utime = time(NULL);
        tmp = localtime(&utime);
        strftime(msg, 256, "%H:%M:%S", tmp);
        send(sock, msg, 256, 0); // отправляем текущее время
    }
        close(sock); // закрываем сокет  
        return 0;
}

