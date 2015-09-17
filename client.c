#include "rudp.h"
#include "udp.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{
    RUDP_Init("localhost", 12348);
    message msg;

    int i;

    for (i=0; i<100; i++) {
        strncpy(msg.cmd, "RUDP_RPC_Test", 24); 
        RUDP_Send_Msg(&msg);
    }

}
