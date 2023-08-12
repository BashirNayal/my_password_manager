#include <stdint.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "log.h"
#include "proto.h"
#include "pwd_err.h"

#define CLIENT_MAX 1


int32_t main(int32_t argc, char **argv) {
    

    
    uint16_t port;      
    struct sockaddr_in server; 
    struct sockaddr_in client;

    if (argc < 2)
    { 
        log_err("MAIN", "Port number was not provided\n");
        return EXIT_FAILURE;
    }

    port = atoi(argv[1]);
    if (port <= 0) 
    {
        log_err("MAIN", "Invalid port provided\n");
        return EXIT_FAILURE;
    }


    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    int32_t server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) 
    {
        log_err("MAIN", "Call to socket() failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    log_info("MAIN", "Created socket at port: %d\n", port);


    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        log_err("MAIN", "Call to bind() failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    if (listen(server_fd, CLIENT_MAX) < 0) 
    {
        log_err("MAIN", "Call to listen() failed: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    log_info("MAIN", "server is now listening for connections...\n");

    pid_t pid = fork();

    log_info("MAIN", "forked a child with pid: %d\n", pid);

    if (pid == 0)
    {
        uint32_t namelen = sizeof(client);
        int32_t client_fd = accept(server_fd, (struct sockaddr *)&client, &namelen);
        if (client_fd < 0) 
        {
            log_err("MAIN", "server failed to connect with client\n");
            return EXIT_FAILURE;
        }
        log_info("MAIN", "Child process accepted a connection\n");
        // Child logic will be moved to a function from a specific file later
        uint8_t buffer[65535];
        uint32_t read_len;
        while (proto_read(client_fd, &buffer,  &read_len) != PWD_FAILURE) 
        {
        
            proto_hdr_st *packet = (proto_hdr_st *)&buffer;
            log_dbg("CHILD", "msg_len: %d, msg_type: %d ", packet->msg_len, packet->msg_type);
            proto_print(packet);
            log_dbg("CHILD", "pwd request with len: %d\n", (proto_args_req_password_st*)packet->data[0]);
            log_info("CHILD", "read_lean: %d\n", read_len);

        }

        log_info("CHILD", "Closing connection with client\n");
        close(client_fd);
        return EXIT_SUCCESS;

    }
    else if(pid < 0) 
    {
        log_err("MAIN", "Call to fork() failed: %s\n", strerror(errno));
        // TODO:Add logic here
        return EXIT_FAILURE;
    }
    else 
    {   
       log_dbg("MAIN", "Waiting for child to terminate...\n");

            int32_t status;
            pid_t child_pid;
            while ((child_pid = waitpid(pid, &status, WNOHANG)) == 0);
            // TODO: handle child status change
            log_info("MAIN", "Child %d changed status\n", child_pid);
       
    }
    

    log_info("MAIN", "closing connection...\n");
    close(server_fd);

    return EXIT_SUCCESS;
}
