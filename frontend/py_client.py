import socket     

MSG_TYPE_INVALID = 0

MSG_TYPE_REQ_LOGIN = 1
MSG_TYPE_REQ_REGISTR = 2
MSG_TYPE_REQ_PWD = 3

# MSG_TYPE_RES_LOGIN = 3
# MSG_TYPE_RES_REGISTR = 4




PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE = 4
PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE = 2
PROTOCOL_HDR_SIZE = PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE + PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE



TEST_USERNAME = "username"
TEST_PWD = "password"




def get_login_request_payload(username, password):

    msg_type = MSG_TYPE_REQ_LOGIN
    msg_len = len(TEST_USERNAME) + len(TEST_PWD) + PROTOCOL_HDR_SIZE + 2
    payload_msg_type = msg_type.to_bytes(PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE, 'little')
    payload_msg_len = msg_len.to_bytes(PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE, 'little')

    payload_hdr = payload_msg_type + payload_msg_len


    return payload_hdr + bytes(username, "utf-8") + b'\0' + bytes(password, "utf-8") + b'\0'

def get_register_request_payload(username, password):

    msg_type = 2
    msg_len = len(TEST_USERNAME) + len(TEST_PWD) + PROTOCOL_HDR_SIZE + 2
    payload_msg_type = msg_type.to_bytes(PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE, 'little')
    payload_msg_len = msg_len.to_bytes(PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE, 'little')

    payload_hdr = payload_msg_type + payload_msg_len

   

    return payload_hdr + bytes(username, "utf-8") + b'\0' + bytes(password, "utf-8") + b'\0'



def get_password_request_payload(pwd_length, has_captials, has_numbers, has_special_chars):

    msg_type = MSG_TYPE_REQ_PWD
    msg_len = 8 + PROTOCOL_HDR_SIZE
    payload_msg_type = msg_type.to_bytes(PROTOCOL_HDR_MSG_TYPE_BYTE_SIZE, 'little')
    payload_msg_len = msg_len.to_bytes(PROTOCOL_HDR_MSG_SIZE_BYTE_SIZE, 'little')



    payload_hdr = payload_msg_type + payload_msg_len

  

    return payload_hdr + pwd_length.to_bytes(1,"little") + b'\0\1\0\1\0\1\0' 


payload = get_password_request_payload(120, 1, 1, 1)
print("client started...")
   

client_socket = socket.socket()        
 
port = 5000               
 
client_socket.connect(('127.0.0.1', port))

print("client connected")

print("sending: ")
print(payload)
client_socket.send(payload)


client_socket.close()    




