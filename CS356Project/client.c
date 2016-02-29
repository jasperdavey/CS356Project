#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

void displayTable( int [ ] );

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    int leastCost[ 7 ] = { 0, 1, 1, 2, 3, 3, 7 };
    char *recvBuff = ( char * )leastCost;
    struct sockaddr_in serv_addr;
    
    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }
    
    displayTable( leastCost );
    
    memset(recvBuff, '0', sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }
    
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    
    if ( send( sockfd, leastCost, sizeof( &leastCost ) / sizeof( int ), 0 ) < 0 )
    {
        printf( "Sending of Least Cost Table failed\n" );
        return 1;
    }
    
    int receivedInt[5];
    size_t arraySize = sizeof( receivedInt ) / sizeof( int );
    if ( read( sockfd, receivedInt, arraySize ) < 0 )
    {
        printf( "Error receiving message from server\n" );
        return 1;
    }
    

    for ( int i = 0; i < arraySize; i ++ )
    {
        printf( "%d\t%d\n", receivedInt[ i ], i );
    }
    
    
    
    if( n < 0 )
    {
        printf("\n Read error \n");
    } 
    
    return 0;
}

void displayTable( int leastCost[ ] )
{
    int arraySize = sizeof( &leastCost ) / sizeof( int );
    printf( "Destination Router\t\tLink Cost\n" );
    printf( "%d\t\t\t\t0\n", leastCost[ 0 ] );
    for ( int x = 1; x < arraySize; x += 2 )
    {
        printf( "%d\t\t\t\t%d\n", leastCost[ x ], leastCost[ x + 1 ] );
    }
    
}





