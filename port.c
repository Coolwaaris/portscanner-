#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    struct addrinfo hints, *result, *p;
    int err, i, sock, start, end;
    char hostname[100];
    struct sockaddr_in* sa;

    // Get the hostname or IP address to scan
    printf("Enter hostname or IP: ");
    fgets(hostname, sizeof(hostname), stdin);
    hostname[strcspn(hostname, "\n")] = '\0';  // Remove newline character if present

    // Get start port number
    printf("\nEnter start port number: ");
    scanf("%d", &start);

    // Get end port number
    printf("Enter end port number: ");
    scanf("%d", &end);

    // Initialize the hints structure
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;

    // Check if the input is already an IP address
    struct sockaddr_in sa_ip;
    if (inet_pton(AF_INET, hostname, &(sa_ip.sin_addr)) == 1)
    {
        sa_ip.sin_family = AF_INET;
        sa = &sa_ip;
    }
    else
    {
        // Resolve hostname to IP address
        if ((err = getaddrinfo(hostname, NULL, &hints, &result)) != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
            exit(1);
        }

        sa = (struct sockaddr_in*)result->ai_addr;
    }

    // Start the port scan loop
    printf("Starting the port scan loop:\n");
    for (i = start; i <= end; i++)
    {
        // Fill in the port number
        sa->sin_port = htons(i);

        // Create a socket of type internet
        sock = socket(AF_INET, SOCK_STREAM, 0);

        // Check whether socket created fine or not
        if (sock < 0)
        {
            perror("Socket");
            exit(1);
        }

        // Connect using that socket and sockaddr structure
        err = connect(sock, (struct sockaddr*)sa, sizeof(struct sockaddr_in));

        // Not connected
        if (err < 0)
        {
            // printf("%s %-5d %s\r", hostname, i, strerror(errno));
            fflush(stdout);
        }
        // Connected
        else
        {
            printf("%-5d open\n", i);
        }

        close(sock);
    }

    if (!sa_ip.sin_addr.s_addr)
        freeaddrinfo(result);

    printf("\r");
    fflush(stdout);
    return 0;
}


