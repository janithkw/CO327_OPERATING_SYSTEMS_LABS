#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void handle_client(int sock)
{
    char buffer[BUFFER_SIZE];
    int n;

    // Clear buffer
    memset(buffer, 0, BUFFER_SIZE);

    // Read file path from client
    n = read(sock, buffer, BUFFER_SIZE - 1);
    if (n < 0)
    {
        error("ERROR reading from socket");
    }

    // Remove newline if present
    buffer[strcspn(buffer, "\n")] = 0;

    // Open the requested file
    FILE *fp = fopen(buffer, "r");
    if (fp == NULL)
    {
        write(sock, "File not found or cannot be opened\n", 36);
        close(sock);
        return;
    }

    // Send file content
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
    {
        write(sock, buffer, strlen(buffer));
    }

    fclose(fp);
    close(sock);
}

int main()
{
    int sockfd, newsockfd, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    // Clear address structure
    memset((char *)&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Listen for connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    printf("Server running on port %d\n", PORT);

    while (1)
    {
        // Accept a client connection
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        pid = fork();
        if (pid < 0)
            error("ERROR on fork");

        if (pid == 0)
        {
            // Child process
            close(sockfd); // Child doesn't need listening socket
            handle_client(newsockfd);
            exit(0);
        }
        else
        {
            // Parent process
            close(newsockfd); // Parent doesn't need client socket
        }
    }

    close(sockfd);
    return 0;
}
