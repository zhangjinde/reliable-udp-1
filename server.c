#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "udp.h"
#include "mfs.h"

message  req;
message resp;
int seq_num = 0;

int test_count = 0;

int main(int argc, char *argv[]) {
    struct sockaddr_in client;
    int port;
    int read_success = 0;
    srand(time(NULL));

    assert(argc == 2);

    port = atoi(argv[1]);

    int fd = UDP_Open(port);

    while (1) {
        UDP_Read(fd, &client, (char *)&req, sizeof(req));

        memset(&resp,0,sizeof(resp));

        if(req.syn == 1) {
            resp.syn = 1;
            resp.ack = 1;
            resp.seq_num = seq_num;
            resp.ack_num = seq_num + 1;
          //  printf("SYN received\n");
            UDP_Write(fd, &client, (char *)&resp, sizeof(resp));
        }

        if(req.ack == 1 && req.ack_num == 1) {
            seq_num = 1;
            printf("Acked %d\n", req.seq_num);
            continue;
        }

        if(seq_num>=1 && req.seq_num == seq_num) {
        //    printf("\n%s %d", req.cmd, req.seq_num);
            test_count++;
            int r = rand()%100;

            if(r>=0 && r<=9) {
                continue;
            } else {
            resp.ack = 1;
            resp.ack_num = seq_num + 1;
            seq_num++;
            read_success = 1;
            UDP_Write(fd, &client, (char *)&resp, sizeof(resp));
            }
        }


        if(read_success) {
            read_success = 0;
            if (!strncmp(req.cmd, "RUDP_RPC_Test", 24)) {
                printf("Server Inited\n");
            } else if (!strncmp(req.cmd, "MFS_Shutdown", 24)) {
                exit(0);
            }
        }
    }

    UDP_Close(fd);
    return 0;
}
