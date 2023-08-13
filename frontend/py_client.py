import socket     

MSG_TYPE_INVALID = 0

MSG_TYPE_REQ_LOGIN = 1
MSG_TYPE_REQ_REGISTR = 2
MSG_TYPE_REQ_PWD = 3
MSG_TYPE_REQ_ADD_ENTRY = 4

# MSG_TYPE_RES_LOGIN = 3
# MSG_TYPE_RES_REGISTR = 4

PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE = 4
PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE = 2
PROTOCOL_HDR_SIZE = PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE + PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE
PROTOCOL_SPEARATOR = b'\0'

TEST_USERNAME = "username"
TEST_PWD = "password"


def get_login_request_payload(username, password):

    msg_type = MSG_TYPE_REQ_LOGIN
    msg_len = len(username) + len(password) + PROTOCOL_HDR_SIZE + 2
    payload_msg_type = msg_type.to_bytes(PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE, 'little')
    payload_msg_len = msg_len.to_bytes(PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE, 'little')
    payload_hdr = payload_msg_type + payload_msg_len
    return payload_hdr + bytes(username, "utf-8") + \
    PROTOCOL_SPEARATOR + bytes(password, "utf-8") + \
    PROTOCOL_SPEARATOR

def get_register_request_payload(username, password):
    msg_type = MSG_TYPE_REQ_REGISTR
    msg_len = len(username) + len(password) + PROTOCOL_HDR_SIZE + 2
    payload_msg_type = \
    msg_type.to_bytes(PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE, 'little')
    payload_msg_len = \
    msg_len.to_bytes(PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE, 'little')
    payload_hdr = payload_msg_type + payload_msg_len
    return payload_hdr + bytes(username, "utf-8") + \
    PROTOCOL_SPEARATOR + bytes(password, "utf-8") + \
    PROTOCOL_SPEARATOR


def get_password_request_payload(pwd_length, has_captials, has_numbers, has_special_chars):

    msg_type = MSG_TYPE_REQ_PWD
    msg_len = 8 + PROTOCOL_HDR_SIZE
    payload_msg_type = msg_type.to_bytes(PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE, 'little')
    payload_msg_len = msg_len.to_bytes(PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE, 'little')
    payload_hdr = payload_msg_type + payload_msg_len
    return payload_hdr + pwd_length.to_bytes(1,"little") + b'\0\1\0\1\0\1\0' 

def get_add_entry_payload(entry_name, username, password, domains_list, description):
    msg_type = MSG_TYPE_REQ_ADD_ENTRY
    msg_len = len(entry_name) + len(username) + \
    len(password) + len(domains_list) + \
    len(description) + PROTOCOL_HDR_SIZE + 5
    payload_msg_len = msg_len.to_bytes(PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE, 'little')
    payload_msg_type = msg_type.to_bytes(PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE, 'little')
    payload_hdr = payload_msg_type + payload_msg_len
    return payload_hdr + bytes(entry_name, "utf-8") + \
    PROTOCOL_SPEARATOR + bytes(username, "utf-8") + \
    PROTOCOL_SPEARATOR + bytes(password, "utf-8") + \
    PROTOCOL_SPEARATOR + bytes(domains_list, "utf-8") + \
    PROTOCOL_SPEARATOR + bytes(description, "utf-8") + \
    PROTOCOL_SPEARATOR

print("client started...")
   

client_socket = socket.socket()        
 
port = 5000               
 
client_socket.connect(('127.0.0.1', port))

print("client connected")


payload = get_password_request_payload(120, 1, 1, 1)
print("sending: ")
print(payload)
client_socket.send(payload)
payload = get_register_request_payload(TEST_USERNAME, TEST_PWD)
print("sending: ")
print(payload)
client_socket.send(payload)
payload = get_login_request_payload(TEST_USERNAME, TEST_PWD)
print("sending: ")
print(payload)
client_socket.send(payload)
payload = get_add_entry_payload("new_entry", \
    TEST_USERNAME, TEST_PWD, \
    "https://test.com,https://google.com", \
    "this is just a description")
print("sending: ")
print(payload)
client_socket.send(payload)

client_socket.close()    




