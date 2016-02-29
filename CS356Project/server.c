#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int leastCost[ 5 ] = { 1, 0, 1, 2, 1 };
    
    char sendBuff[1025];
    time_t ticks;
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);
    
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    listen(listenfd, 10);
    
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
        
        if ( connfd )
        {
            printf( "Sending Least Cost Table\n" );
            
            if ( ( write(connfd, leastCost, sizeof( &leastCost ) / sizeof( int ) ) ) < 0 )
            {
                printf( "Error sending Least Cost Table\n" );
                return 1;
            }
            
            //printf( "Least Cost Table sent\n" );
        }
        
        close(connfd);
        sleep(1);
    }
}