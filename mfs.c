#include "mfs.h"
#include "udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

//message req_msg;
//message resp_msg;

char host[200];
int port_num;
int seq_num = 0;

int RUDP_Send_Msg(message *req) {
    message resp;
    req->seq_num = seq_num;
    if(RUDP_Send(req, &resp, 1)) {
        return 0;
    } else {
        return 1;
    }
}    

int RUDP_Send(message *req, message *resp, int waitForAck) {

    struct sockaddr_in server;
    struct sockaddr_in other;

    UDP_FillSockAddr(&server, host, port_num);

    int fd = UDP_Open(0);

    int retransmit = 0;
    int ready;
    int prev_seq_num = seq_num;

    do {

        UDP_Write(fd, &server, (char *)req, sizeof(message));
 
  
//    resp = malloc(sizeof(message));
//
        if(waitForAck) {
            seq_num = prev_seq_num + 1;
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 500;

            fd_set   fds;
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            if ((ready = select(fd+1, &fds, NULL, NULL, &tv))) {
                UDP_Read(fd, &other, (char *)resp, sizeof(message));

                if (resp->ack == 1 && resp->ack_num == seq_num) {
                    //Received proper ack
                    retransmit = 0;
                } else {
                    printf("Improper: %d\n", resp->ack_num);
                    retransmit = 1;
                }
            } else {
                retransmit = 1;
            }  

        }

        printf("Retransmit: %d Ack: %d\n", retransmit, resp->ack_num);
    } while (retransmit!=0);
    UDP_Close(fd);

    return 0;
}

int RUDP_Init(char *hostname, int port)
{
    int rc = 0;;
    strncpy(host, hostname, 200);
    port_num = port;
    message req_msg, resp_msg;
  
    memset(&req_msg, 0, sizeof(message)); 
    strncpy(req_msg.cmd, "RUDP_Init", 24);

    seq_num = 0;
    req_msg.syn = 1;
    req_msg.seq_num = seq_num;
    rc = RUDP_Send(&req_msg, &resp_msg, 1);
    
    if(rc==0 && resp_msg.syn == 1 && resp_msg.ack == 1 && resp_msg.ack_num == seq_num) {
        memset(&req_msg, 0, sizeof(message));
        req_msg.ack = 1;
        req_msg.seq_num = seq_num;
        req_msg.ack_num = resp_msg.seq_num + 1;
        printf("Ack Sent\n");
        rc = RUDP_Send(&req_msg, &resp_msg, 0);
    } else {
        rc = 1;
    }
  
    return rc;
}
/*
int RUDP_Shutdown()
{
    int rc;
    strncpy(req_msg.cmd, "MFS_Shutdown", 64);
    rc = MFS_UDP(&req_msg);
    return rc;
}*/
