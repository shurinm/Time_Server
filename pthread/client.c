#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main (int argc, char **argv)
{
	int sock;								//дискрипторы сокетов
	struct sockaddr_in addr;				//структура с адресом
	char buf[2048];       					//буфур для приема
    int bytes_read = 0;           			//кол-во принятых байт
    char msg[256];							//сообщение

	sock = socket(AF_INET, SOCK_STREAM, 0);	//создание TCP-сокета
	if(sock < 0)
    {
        perror("socket");
        exit(-1);
    }
    //задаем параметры сервера
    addr.sin_family = AF_INET; // домены Internet
    addr.sin_port = htons(3428); // или любой другой порт...
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) // установка соединения с сервером
    {
        perror("Подключение");
        exit(-1);
    }
     while (1){
        // Считываем и отправляем
        printf("Cообщение: ");
        scanf("%s", msg);
        if (!strcmp(msg, "exit"))
            break;
        send(sock, msg, strlen(msg) + 1, 0);

        //Принимаем
        if (recv(sock, msg, 256, 0) > 0)
            printf("Вернул: %s\n", msg);
    }
     // Завершение
    close(sock);
    exit(0);
}