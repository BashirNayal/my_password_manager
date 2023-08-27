#include "server_worker.h"
#include "log.h"
#include "proto.h"
#include "pwd_err.h"

pwd_err_et handle_client_communication(int32_t client_fd)
{
    uint8_t buffer[MSG_ENC_SIZE_MAX];
    uint32_t read_len;

    while (proto_read(client_fd, &buffer,  &read_len) != PWD_FAILURE) 
    {
        proto_hdr_st *packet = (proto_hdr_st *)&buffer;
        proto_print(packet);
        handle_client_request(client_fd, packet);
    }
    log_info("C_COM", "Closing connection with client\n");

}

pwd_err_et handle_client_request(int32_t client_fd, proto_hdr_st *packet)
{
    log_dbg("WORKER", "handling request\n");

    switch(packet->msg_type) 
    {
        case MSG_TYPE_REQ_LOGIN:
        {
            log_dbg("WORKER", "handling login request\n");
            break;
        }
        case MSG_TYPE_REQ_REGISTR:
        {
            log_dbg("WORKER", "handling register request\n");
            break;
        }
        case MSG_TYPE_REQ_NEW_PWD:
        {
            log_dbg("WORKER", "handling new pwd request\n");
            break;
        }
        case MSG_TYPE_REQ_ADD_ENTRY:
        {
            log_dbg("WORKER", "handling add entry request\n");
            break;
        }
        case MSG_TYPE_REQ_MODIFY_ENTRY:
        {
            log_dbg("WORKER", "handling modify entry request\n");
            break;
        }
        case MSG_TYPE_REQ_GET_ENTRY_LIST:
        {
            log_dbg("WORKER", "handling get entry request\n");
            break;
        }

    }

    return PWD_FAILURE;
}