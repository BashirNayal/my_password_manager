#include "pwd_err.h"
#include "log.h"
#include "util.h"
#include "proto.h"

pwd_err_et handle_client_communication(int32_t client_fd);
pwd_err_et handle_client_request(int32_t client_fd, proto_hdr_st *packet);
