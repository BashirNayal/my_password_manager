#pragma once

#include <stdint.h>
#include "pwd.h"
#include "pwd_err.h"

#define PROTO_SEPARATOR_CHAR '\0'

#define MSG_ENC_SIZE_MAX UINT16_MAX
#define MSG_DEC_SIZE_MAX ((UINT16_MAX >> 2) * 3)

/* Argument size bounds (may be useful elsewhere for static allocs). These
 * lengths are for the encoded versions i.e. how they appear in the packet on
 * the network. */
#define USERNAME_LEN_MAX 256
#define USERNAME_LEN_MIN 2
#define PASSWORD_LEN_MAX 512
#define PASSWORD_LEN_MIN 2
#define MSG_LEN_MIN 1


typedef enum msg_type_s
{
    MSG_TYPE_INVALID = 0,

    MSG_TYPE_REQ_LOGIN,
    MSG_TYPE_REQ_REGISTR,

    MSG_TYPE_RES_LOGIN,
    MSG_TYPE_RES_REGISTR,
    MSG_TYPE_LEN,
} msg_type_st;

typedef enum msg_status_s
{
    MSG_STATUS_FAILURE,
    MSG_STATUS_SUCCESS,
} msg_status_st;

typedef struct proto_hdr_s
{
    msg_type_st msg_type;
    uint16_t msg_len;
    uint8_t data[];
} __attribute__((packed)) proto_hdr_st;

/* These "args" structs contain a description of the "data" field in the
 * message. */
typedef struct proto_args_req_login_s
{
    uint16_t username_idx_start;
    uint16_t username_len;
    uint16_t password_idx_start;
    uint16_t password_len;
} proto_args_req_login_st;
typedef struct proto_args_req_registr_s
{
    uint16_t username_idx_start;
    uint16_t username_len;
    uint16_t password_idx_start;
    uint16_t password_len;
} proto_args_req_registr_st;


/* All the response structs have a status variable which indicates whether or
 * not the server accepted the request/data sent by the client. These response
 * structs will be stored in the data[] field of proto_hdr_st along with the
 * data sent by the server. */
typedef struct proto_args_res_login_s
{
    msg_status_st status;
} proto_args_res_login_st;
typedef proto_args_res_login_st proto_args_res_registr_st;

/* This is used to represent a complete parsed message but to avoid copying the
 * original message, the args of course only refer to parts of the original
 * message "data" (using a byte-index) so the original message must not be
 * freed. */
typedef struct proto_parsed_s
{
    msg_type_st msg_type;
    union {
        proto_args_req_login_st req_login;
        proto_args_req_registr_st req_registr;

        proto_args_res_login_st res_login;
        proto_args_res_registr_st res_registr;
    } data_args;
} proto_parsed_st;

/**
 * @brief Read a whole message from the given file descriptor into the buffer
 *  and update the "read_len" with the number of bytes that were read from FD.
 * @note This functions makes all the necessary checks on the message format to
 *  never return successfully if the message format is not correct. The data
 *  portion of the message is not checked.
 * @param fd Where message will be read from.
 * @param ssl The SSL struct associated with the file descriptor.
 * @param buffer Where the message will be written to.
 * @param read_len On success, the number of bytes read, will be written here.
 */
pwd_err_et proto_read(int fd, /*SSL *ssl,*/ uint8_t (*buffer)[MSG_ENC_SIZE_MAX],
                        uint32_t *read_len);

/**
 * @brief Transmit a packet.
 * @param fd File descriptor associated with the socket.
 * @param ssl The SSL struct associated with the file descriptor.
 * @param packet The packet that should be sent out.
 */
pwd_err_et proto_write(int fd, /*SSL *ssl,*/ proto_hdr_st *packet);

/**
 * @brief Parse a packet.
 * @param packet A raw packet received from the network. The caller must
 *  guarantee that the size of the buffer (whole packet + data) is equal to the
 *  message length field and that the message type is valid.
 * @param parsed Information about where to find arguments in the data buffer of
 *  the packet which has been extracted at parse-time and arguments have been
 *  checked for validity.
 */
pwd_err_et proto_parse(proto_hdr_st *packet, proto_parsed_st *parsed);

void proto_print(proto_hdr_st *packet);
