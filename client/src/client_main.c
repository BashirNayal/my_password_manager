#include <stdint.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

#include "log.h"

int32_t main(int32_t argc, char **argv) {


    struct hostent *host_name;   
    uint16_t port;      
    struct sockaddr_in server; 
    port = 5000;
    /*
     * Check Arguments Passed. Should be hostname and port.
     */

    /*
     * The host name is the first argument. Get the server address.
     */
    host_name = gethostbyname("localhost");

    if (host_name == (struct hostent *) 0)
    {
        fprintf(stderr, "Gethostbyname failed\n");
    }
    

    // /*
    //  * Bind the socket to the server address.
    //  */

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)host_name->h_addr);


    int32_t client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0) 
    {
        // Handle error
    }

    int32_t connect_result = connect(client_fd, (struct sockaddr *)&server, sizeof(server));
    if (connect_result < 0) 
    {
        // Handle error
    }

    printf("client file descriptor: %d\n" , client_fd);

    close(client_fd);

    return 0;
}
