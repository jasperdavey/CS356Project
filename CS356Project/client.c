/*  client.c
 *  CS 356 Stage One
 *  Author: Jasper Davey
 */

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

void displayTable( int[ ], size_t );
void updateTable( int [ ], int [ ], size_t, size_t );

int main( int argc, char *argv[ ] )
{
    int sockfd = 0, n = 0;
    static int leastCost[ 7 ] = { 0, 1, 1, 2, 3, 3, 7 };
    struct sockaddr_in serv_addr;
    
    if ( argc != 2 )
    {
        printf( "\n Usage: %s <ip of server> \n", argv[ 0 ] );
        return 1;
    }
    
    displayTable( leastCost, sizeof( leastCost ) );
    
    if ( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        printf( "\n Error : Could not create socket \n" );
        return 1;
    }
    
    memset( &serv_addr, '0', sizeof( serv_addr ) );
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( 5000 );
    
    if ( inet_pton( AF_INET, argv[ 1 ], &serv_addr.sin_addr ) <= 0 )
    {
        printf( "\n inet_pton error occured\n" );
        return 1;
    }
    
    if ( connect( sockfd, ( struct sockaddr * )&serv_addr, sizeof( serv_addr ) ) < 0 )
    {
        printf( "\n Error : Connect Failed \n" );
        return 1;
    }
    
    int sendLeastCost[ 7 ];
    for ( int x = 0; x < sizeof( sendLeastCost ) / sizeof( int ); x++ )
    {
        sendLeastCost[ x ] = htonl( leastCost[ x ] );
    }
    
    if ( send( sockfd, sendLeastCost, 7 * sizeof( int ), 0 ) < 0 )
    {
        printf( "Sending of Least Cost Table failed\n" );
        return 1;
    }
    
    int serverResponse[ 5 ];
    int receivedInt[ 5 ];
    if ( ( read( sockfd, serverResponse, 5 * sizeof( int ) ) )  < 0 )
    {
        printf( "Error receiving message from server\n" );
        return 1;
    }
    
    for ( int x = 0; x < sizeof( serverResponse ) / 4; x ++ )
    {
        receivedInt[ x ] = ntohl( serverResponse[ x ] );
    }
    
    displayTable( receivedInt, sizeof( receivedInt ) );
    updateTable( leastCost, receivedInt, sizeof( leastCost ), sizeof( receivedInt ) );
    displayTable( leastCost, sizeof( leastCost ) );
    
    if ( n < 0 )
    {
        printf("\n Read error \n");
    } 
    
    return 0;
}

void displayTable( int leastCost[ ], size_t arraySize )
{

    printf( "Router 0 Destination Router\t\tLink Cost\n" );
    printf( "%d\t\t\t\t0\n", leastCost[ 0 ] );
    for ( int x = 1; x < ( arraySize / sizeof( int ) ) - 1; x += 2 )
    {
        printf( "%d\t\t\t\t%d\n", leastCost[ x ], leastCost[ x + 1 ] );
    }
    
}

void updateTable( int leastCost[ ], int receivedLeastCost[ ], size_t leastCostSize, size_t receivedLeastCostSize )
{
    printf( "Updating Router 1 Least Cost Table\n" );
    
    for ( int x = 1; x < ( receivedLeastCostSize / sizeof( int ) ) - 1; x += 2 )
    {
        for ( int y = 1; y < ( leastCostSize / sizeof( int ) ) - 1; y += 2 )
        {
            if ( leastCost[ y ] == receivedLeastCost[ x ] )
            {
                if ( receivedLeastCost[ x + 1 ] > leastCost[ y + 1 ] )
                {
                    leastCost[ y + 1 ] = receivedLeastCost[ x + 1 ];
                }
            }
        }
    }
}