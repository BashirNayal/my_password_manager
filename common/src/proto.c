#include "proto.h"
#include "log.h"
#include "pwd_err.h"
#include "util.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static pwd_err_et parse_empty(proto_hdr_st *packet)
{
    if (packet->msg_len != sizeof(proto_hdr_st) + 1 &&
        packet->data[0] == PROTO_SEPARATOR_CHAR)
    {
        return PWD_FAILURE;
    }
    return PWD_SUCCESS;
}


static pwd_err_et parse_req_login(proto_hdr_st *packet,
                                    proto_args_req_login_st *data_parsed)
{
    uint16_t separators[2] = {0};
    if (separators_find(packet->data,
                        packet->msg_len - (uint16_t)sizeof(proto_hdr_st), 2,
                        separators, PROTO_SEPARATOR_CHAR, 1) == PWD_FAILURE)
    {
        log_err("PROTO", "Too many or too few separators in data\n");
        return PWD_FAILURE;
    }

    // At this point we know where arguments start and end in data of packet.
    uint16_t len_username = separators[0] - 0;
    uint16_t len_password = (uint16_t)(separators[1] - (separators[0] + 1));

    if (len_username < USERNAME_LEN_MIN || len_username > USERNAME_LEN_MAX)
    {
        log_err("PROTO", "Username length invalid\n");
        return PWD_FAILURE;
    }
    if (len_password < PASSWORD_LEN_MIN || len_password > PASSWORD_LEN_MAX)
    {
        log_err("PROTO", "Password length invalid\n");
        return PWD_FAILURE;
    }

    data_parsed->username_idx_start = 0;
    data_parsed->username_len = len_username;
    data_parsed->password_idx_start = separators[0] + 1;
    data_parsed->password_len = len_password;

    return PWD_SUCCESS;
}


static pwd_err_et parse_req_registr(proto_hdr_st *packet,
                                      proto_args_req_registr_st *data_parsed)
{
    uint16_t separators[2] = {0};
    if (separators_find(packet->data,
                        packet->msg_len - (uint16_t)sizeof(proto_hdr_st), 2,
                        separators, PROTO_SEPARATOR_CHAR, 1) == PWD_FAILURE)
    {
        log_err("PROTO", "Too many or too few separators in data\n");
        return PWD_FAILURE;
    }

    // At this point we know where arguments start and end in data of packet.
    uint16_t len_username = separators[0] - 0;
    uint16_t len_password = (uint16_t)(separators[1] - (separators[0] + 1));

    if (len_username < USERNAME_LEN_MIN || len_username > USERNAME_LEN_MAX)
    {
        log_err("PROTO", "Username length invalid\n");
        return PWD_FAILURE;
    }
    if (len_password < PASSWORD_LEN_MIN || len_password > PASSWORD_LEN_MAX)
    {
        log_err("PROTO", "Password length invalid\n");
        return PWD_FAILURE;
    }

    data_parsed->username_idx_start = 0;
    data_parsed->username_len = len_username;
    data_parsed->password_idx_start = separators[0] + 1;
    data_parsed->password_len = len_password;



    return PWD_SUCCESS;
}
static pwd_err_et parse_req_new_password(proto_hdr_st *packet,
                                            proto_args_req_new_password_st *data_parsed)
{
        uint16_t separators[4] = {0};
    if (separators_find(packet->data,
                        packet->msg_len - (uint16_t)sizeof(proto_hdr_st), 4,
                        separators, PROTO_SEPARATOR_CHAR, 1) == PWD_FAILURE)
    {
        log_err("PROTO", "Too many or too few separators in data\n");
        return PWD_FAILURE;
    }

    data_parsed->pwd_length_idx = 0;
    data_parsed->has_capital_letters_idx = separators[0] + 1;
    data_parsed->has_numbers_idx = separators[1] + 1;
    data_parsed->has_special_characters_idx = separators[2] + 1;

    return PWD_SUCCESS;
}
static pwd_err_et parse_req_add_entry(proto_hdr_st *packet,
                                            proto_args_req_add_entry_st *data_parsed)
{
        uint16_t separators[5] = {0};
    if (separators_find(packet->data,
                        packet->msg_len - (uint16_t)sizeof(proto_hdr_st), 5,
                        separators, PROTO_SEPARATOR_CHAR, 1) == PWD_FAILURE)
    {
        log_err("PROTO", "Too many or too few separators in data\n");
        return PWD_FAILURE;
    }

    uint16_t len_entry_name = separators[0] - 0;
    uint16_t len_username = (uint16_t)(separators[1] - (separators[0] + 1));
    uint16_t len_password = (uint16_t)(separators[2] - (separators[1] + 1));
    uint16_t len_domains_list = (uint16_t)(separators[3] - (separators[2] + 1));
    uint16_t len_description = (uint16_t)(separators[4] - (separators[3] + 1));

    data_parsed->entry_name_idx_start = 0;
    data_parsed->entry_name_len = len_entry_name;
    data_parsed->username_idx_start = separators[0] + 1;
    data_parsed->username_len = len_username;
    data_parsed->password_idx_start = separators[1] + 1;
    data_parsed->password_len = len_password;
    data_parsed->domains_list_idx_start = separators[2] + 1;
    data_parsed->domains_list_len = len_domains_list;
    data_parsed->description_idx_start = separators[3] + 1;
    data_parsed->description_len = len_description;

    return PWD_SUCCESS;
}



static pwd_err_et parse_res_login(proto_hdr_st *packet,
                                    proto_args_res_login_st *data_parsed)
{
    if (packet->msg_len != sizeof(proto_hdr_st) + sizeof(msg_status_st) + 1)
    {
        log_err("PROTO", "Packet length too short\n");
        return PWD_FAILURE;
    }
    if (packet->data[packet->msg_len - 1 - sizeof(proto_hdr_st)] !=
        PROTO_SEPARATOR_CHAR)
    {
        log_err("PROTO", "Last byte is not a separator\n");
        return PWD_FAILURE;
    }

    msg_status_st *status = (msg_status_st *)&packet->data[0];

    data_parsed->status = *status;

    return PWD_SUCCESS;
}

static pwd_err_et parse_res_registr(proto_hdr_st *packet,
                                      proto_args_res_registr_st *data_parsed)
{
    return parse_res_login(packet, data_parsed);
}

pwd_err_et proto_read(int fd, uint8_t (*buffer)[MSG_ENC_SIZE_MAX],
                        uint32_t *read_len) // , int blocking)
{
    proto_hdr_st *packet = (proto_hdr_st *)buffer;

    ssize_t read_len_tmp = 0;

    read_len_tmp = read(fd, buffer, sizeof(proto_hdr_st));
    
    if (read_len_tmp <= -1)
    {
        log_err("PROTO", "Call to read() failed: %s\n", strerror(errno));
        return PWD_FAILURE;
    }
    if (read_len_tmp == 0)
    {
        log_warn("PROTO", "Got EOF from socket\n");
        *read_len = 0;
        return PWD_FAILURE;
    }

    if (packet->msg_type == MSG_TYPE_INVALID ||
        packet->msg_type >= MSG_TYPE_LEN)
    {
        log_warn("PROTO", "Received unknown/unexpected message type: %u\n",
                 packet->msg_type);
        return PWD_FAILURE;
    }

    if (packet->msg_len < sizeof(proto_hdr_st))
    {
        log_warn("PROTO", "Packet has invalid length: %u\n", packet->msg_len);
        return PWD_FAILURE;
    }
    /* At this point "msg_len" needs to be some number between
     * sizeof(proto_hdr_st) and UINT16_MAX (inclusive). */

    // At this point, "read_len_tmp" is definately a positive number.
    *read_len = (uint32_t)read_len_tmp;

    uint16_t data_len = packet->msg_len - (uint16_t)sizeof(proto_hdr_st);
    read_len_tmp = 0;
    while (data_len > 0)
    {
        /* TODO: This may block here forever. Use non-blocking IO, e.g.
         * "fcntl(fd, F_GETFL, 0)". */
        read_len_tmp =
            read(fd, packet->data + (*read_len) - sizeof(proto_hdr_st),
                    data_len);
        
        if (read_len_tmp <= -1)
        {
            log_err("PROTO", "Reading from socket failed: %s\n", strerror(errno));
            return PWD_FAILURE;
        }
        if (read_len_tmp == 0)
        {
            log_warn("PROTO", "Got EOF from socket\n");
            *read_len = 0;
            return PWD_FAILURE;
        }

        if ((int64_t)data_len - (int64_t)read_len_tmp < 0)
        {
            log_err(
                "PROTO",
                "Call to read() returned too many bytes: %zd vs. %u\n",
                read_len_tmp, data_len);
            return PWD_FAILURE;
        }
        data_len -= (uint16_t)read_len_tmp;
        *read_len += (uint16_t)read_len_tmp;
    }
    // At this point we got a complete header and data so reading is done!
    return PWD_SUCCESS;
}

pwd_err_et proto_write(int fd, proto_hdr_st *packet)
{
    ssize_t written = 0;
    ssize_t written_tmp = 0;
    while (written != packet->msg_len)
    {

        written_tmp = write(fd, &((uint8_t *)packet)[written],
                            packet->msg_len - (uint16_t)written);

        if (written_tmp <= -1)
        {
            log_err("PROTO", "Writing to socket failed: %s\n", strerror(errno));
            return PWD_FAILURE;
        }
        written += written_tmp;
    }
    return PWD_SUCCESS;
}

pwd_err_et proto_parse(proto_hdr_st *packet, proto_parsed_st *parsed)
{
    if (packet->msg_len < sizeof(proto_hdr_st))
    {
        log_err("PROTO", "Packet message length is too short\n");
        return PWD_FAILURE;
    }

    parsed->msg_type = packet->msg_type;
    switch (packet->msg_type)
    {
    case MSG_TYPE_REQ_LOGIN: {
        proto_args_req_login_st data_parsed = {0};
        if (parse_req_login(packet, &data_parsed) == PWD_FAILURE)
        {
            log_err("PROTO", "Parsing login message request failed\n");
            return PWD_FAILURE;
        }
        parsed->data_args.req_login = data_parsed;
        return PWD_SUCCESS;
    }
    case MSG_TYPE_REQ_REGISTR: {
        proto_args_req_registr_st data_parsed = {0};
        if (parse_req_registr(packet, &data_parsed) == PWD_FAILURE)
        {
            log_err("PROTO", "Parsing register message request failed\n");
            return PWD_FAILURE;
        }
        parsed->data_args.req_registr = data_parsed;
        return PWD_SUCCESS;
    }
    case MSG_TYPE_REQ_NEW_PWD: {
        proto_args_req_new_password_st data_parsed = {0};
        if (parse_req_new_password(packet, &data_parsed) == PWD_FAILURE)
        {
            log_err("PROTO", "Parsing password request failed\n");
            return PWD_FAILURE;
        }
        parsed->data_args.req_new_pwd = data_parsed;
        return PWD_SUCCESS;
    }
    case MSG_TYPE_REQ_ADD_ENTRY: {
        proto_args_req_add_entry_st data_parsed = {0};
        if (parse_req_add_entry(packet, &data_parsed) == PWD_FAILURE)
        {
            log_err("PROTO", "Parsing password request failed\n");
            return PWD_FAILURE;
        }
        parsed->data_args.req_add_entry = data_parsed;
        return PWD_SUCCESS;

    }
    default:
        log_err("PROTO", "Parsing failed due to invalid message type\n");
        return PWD_FAILURE;
    }
    
}

void proto_print(proto_hdr_st *packet)
{
    log_info("PROTO",
             "Packet:"
             "\n\t len: " CLR_VAR("%u") "\n\ttype: " CLR_VAR("%u") "\n\tdata: ",
             packet->msg_len, packet->msg_type);

#ifdef DEBUG
    uint16_t data_len = packet->msg_len - (uint16_t)sizeof(proto_hdr_st);

    // Print raw representation first.
    for (uint16_t data_idx = 0; data_idx < data_len; ++data_idx)
    {
        if (packet->data[data_idx] >= 21 && packet->data[data_idx] <= 126)
        {
            printf(CLR_VAR("%c"), packet->data[data_idx]);
        }
        else
        {
            printf(CLR_TXT(CLR_MAG, "\\x%x"), packet->data[data_idx]);
        }
    }
    printf("\n");

    proto_parsed_st packet_parsed = {0};
    pwd_err_et err = proto_parse(packet, &packet_parsed);
    if (err == PWD_FAILURE)
    {
        // printf(CLR_TXT(CLR_RED, "\tPacket data is invalid"));
        return;
    }
    else
    {
        printf(CLR_TXT(CLR_GRN, "\tPacket data is valid"));
        switch (packet_parsed.msg_type)
        {
        case MSG_TYPE_REQ_LOGIN: {
            const proto_args_req_login_st *data =
                &packet_parsed.data_args.req_login;
            printf("\n\tusername: \"%.*s\""
                   "\n\tpassword: \"%.*s\"",
                   data->username_len, &packet->data[data->username_idx_start],
                   data->password_len, &packet->data[data->password_idx_start]);
            break;
        }
        case MSG_TYPE_REQ_REGISTR: {
            const proto_args_req_registr_st *data =
                &packet_parsed.data_args.req_registr;
            printf("\n\tusername: \"%.*s\""
                   "\n\tpassword: \"%.*s\"",
                   data->username_len, &packet->data[data->username_idx_start],
                   data->password_len, &packet->data[data->password_idx_start]);
            break;
        }
        case MSG_TYPE_REQ_NEW_PWD: {
            const proto_args_req_new_password_st *data =
                &packet_parsed.data_args.req_new_pwd;
            printf("\n\tpassword length: \"%d\""
                   "\n\thas captials: \"%u\""
                   "\n\thas numbers: \"%d\""
                   "\n\thas special chars: \"%u\"",
                    packet->data[0], 
                    packet->data[data->has_capital_letters_idx], 
                    packet->data[data->has_numbers_idx], 
                    packet->data[data->has_special_characters_idx]);

            break;
        }
        case MSG_TYPE_REQ_ADD_ENTRY: {
            const proto_args_req_add_entry_st *data =
                &packet_parsed.data_args.req_add_entry;    
            printf("\n\tentry name: \"%.*s\""
                   "\n\tusername: \"%.*s\""
                   "\n\tpassword: \"%.*s\""
                   "\n\tdomains list: \"%.*s\""
                   "\n\tdescription: \"%.*s\"",
                   data->entry_name_len, &packet->data[data->entry_name_idx_start],
                   data->username_len, &packet->data[data->username_idx_start],
                   data->password_len, &packet->data[data->password_idx_start],
                   data->domains_list_len, &packet->data[data->domains_list_idx_start],
                   data->description_len, &packet->data[data->description_idx_start]);        
        }
            // Nothing extra to show.
            break;
        }
    }
    printf("\n");
#else
    (void)packet; // Unused.
#endif
}

