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

void displayTable( int [ ] );

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int leastCost[ 5 ] = { 1, 0, 1, 2, 1 };
    int receivedInt[ 7 ];
    
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
            if ( ( read( connfd, receivedInt, sizeof( leastCost ) / sizeof( int ) ) ) < 0 )
            {
                printf( "Error reading Least Cost Table\n" );
                return 1;
            }
            
            displayTable( receivedInt );
            
            printf( "Sending Least Cost Table\n" );
            size_t arraySize = sizeof( leastCost ) / sizeof( leastCost[0] );
            if ( ( write(connfd, leastCost, arraySize ) ) < 0 )
            {
                printf( "Error sending Least Cost Table\n" );
                return 1;
            }
            
        }
        
        printf( "Least Cost Table sent\n" );
        close(connfd);
        sleep(1);
    }
}

void displayTable( int leastCost[ ] )
{
    int arraySize = sizeof( &leastCost );
    printf( "Destination Router\t\tLink Cost\n" );
    printf( "%d\t\t\t\t0\n", leastCost[ 0 ] );
    for ( int x = 1; x < arraySize - 1; x += 2 )
    {
        printf( "%d\t\t\t\t%d\n", leastCost[ x ], leastCost[ x + 1 ] );
    }
    
}