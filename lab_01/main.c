#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#define STR1 "How are you from child 1?"
#define STR2 "How are you from child 2?"
#define ANSWER1 "I'm ok, thank you to child 1."
#define ANSWER2 "I'm ok, thank you to child 2."
#define BUF_SIZE 1024
int main(int argc, char ** argv)
{ 
    // массив файловых дескрипторов: файловые дескрипторы
    int sockets[2];

    // Буфер для получения сообщения
    char buf[BUF_SIZE];

    // сообщение от дочерних процессов
    char message[2][BUF_SIZE] = {STR1, STR2};

    // сообщение от родительского процесса
    char answer[2][BUF_SIZE] = {ANSWER1, ANSWER2};
    int children[2];
    for (int i = 0; i < 2; i++)
    {
        // Создать парные сокеты
        // socketpair поддерживает только AF_UNIX
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) 
        {
            perror("socketpair() failed.\n");
            return EXIT_FAILURE;
        }
        children[i] = fork();
        if (children[i] == -1)
        {
            perror("fork() failed.\n");
            return EXIT_FAILURE;
        }
        // Если это дочерный процесс
        if (children[i] == 0) 
        {
            close(sockets[1]);
            printf("Child %d sent message.\n", i + 1);
            write(sockets[0], message[i], sizeof(message[i]));
            read(sockets[0], buf, sizeof(buf));
            printf("Child %d received message: %s\n\n", i + 1, buf);
            close(sockets[0]);
            // Завершить дочерный процесс, чтобы 2-й раз fork не в 1-й дочернем процессе.
            break;
        } 
        // родительский процесс
        else 
        {
            close(sockets[0]);
            read(sockets[1], buf, sizeof(buf));
            printf("Parents received message: %s\n", buf);
            printf("Parents sent message.\n");
            write(sockets[1], answer[i], sizeof(answer[i]));
            close(sockets[1]); 
        }
    } 
    return 0;
}