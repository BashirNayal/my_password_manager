#include <stdint.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

int32_t main(int32_t argc, char **argv) {
    
    uint16_t port;      
    struct sockaddr_in server; 
    struct sockaddr_in client;
    port = 5000;
    // /*
    //  * Bind the socket to the server address.
    //  */


    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    int32_t server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) 
    {
        // Handle error
    }

    int32_t bind_result = bind(server_fd, (struct sockaddr *)&server, sizeof(server));

    if (bind_result < 0)
    {
        // Handle error
    }
    
    int32_t listen_result = listen(server_fd, 1);

    if (listen_result < 0) 
    {
        // Handle error
    }

    uint32_t namelen = sizeof(client);
    int32_t client_fd = accept(server_fd, (struct sockaddr *)&client, &namelen);

    if (client_fd < 0) 
    {
        // Handle error
    }

    printf("client file descriptor: %d\n" , client_fd);

    close(client_fd);
    close(server_fd);

    return 0;
}