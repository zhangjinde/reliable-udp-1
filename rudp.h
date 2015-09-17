#include<stdint.h>

typedef struct __attribute__((__packed__)) __message__ {
        uint32_t seq_num;
        uint32_t ack_num;
        uint32_t syn : 1;
        uint32_t fin : 1;
        uint32_t ack : 1;
        uint32_t unused : 29; 
        char cmd[24];
        uint8_t args[0];
} message;

int RUDP_Init(char *hostname, int port);
int RUDP_Send(message *req, message *resp, int waitForAck);
int RUDP_Send_Msg(message *req);
int RUDP_Shutdown(); 

