/*  client.c
 *  CS 356 Stage Three
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
    static int leastCost[ 10 ] = { 0, 1, 0, 1, 2, 1, 3, 3, 2, 7 };
    struct sockaddr_in serv_addr;
    
    if ( argc != 2 )
    {
        printf( "\n Usage: %s <ip of server> \n", argv[ 0 ] );
        return 1;
    }
    
    
    
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
    
    int sendLeastCost[ 10 ];
    for ( int x = 0; x < sizeof( sendLeastCost ) / sizeof( int ); x++ )
    {
        sendLeastCost[ x ] = htonl( leastCost[ x ] );
    }
    
    if ( send( sockfd, sendLeastCost, sizeof( sendLeastCost ) * sizeof( int ), 0 ) < 0 )
    {
        printf( "Sending of Least Cost Table failed\n" );
        return 1;
    }
    
    int serverResponse[ 10 ];
    int receivedInt[ 10 ];
    if ( ( read( sockfd, serverResponse, sizeof( receivedInt ) * sizeof( int ) ) )  < 0 )
    {
        printf( "Error receiving message from server\n" );
        return 1;
    }
    
    for ( int x = 0; x < sizeof( serverResponse ) / sizeof( int ); x ++ )
    {
        receivedInt[ x ] = ntohl( serverResponse[ x ] );
    }
    
    printf( "Displaying initial least cost table\n" );
    displayTable( leastCost, sizeof( leastCost ) );
    
    updateTable( leastCost, receivedInt, sizeof( leastCost ), sizeof( receivedInt ) );
    
    printf( "Displaying updated initial least cost table\n" );
    displayTable( leastCost, sizeof( leastCost ) );
    
    if ( n < 0 )
    {
        printf("\n Read error \n");
    }
    
    
    // Router 2
    if ( inet_pton( AF_INET, "10.0.2.15", &serv_addr.sin_addr ) <= 0 )
    {
        printf( "\n inet_pton error occured\n" );
        return 1;
    }
    
    if ( connect( sockfd, ( struct sockaddr * )&serv_addr, sizeof( serv_addr ) ) < 0 )
    {
        printf( "\n Error : Connect Failed \n" );
        return 1;
    }
    
    for ( int x = 0; x < sizeof( sendLeastCost ) / sizeof( int ); x++ )
    {
        sendLeastCost[ x ] = htonl( leastCost[ x ] );
    }
    
    if ( send( sockfd, sendLeastCost, sizeof( sendLeastCost ) * sizeof( int ), 0 ) < 0 )
    {
        printf( "Sending of Least Cost Table failed\n" );
        return 1;
    }
    
    if ( ( read( sockfd, serverResponse, sizeof( receivedInt ) * sizeof( int ) ) )  < 0 )
    {
        printf( "Error receiving message from server\n" );
        return 1;
    }
    
    for ( int x = 0; x < sizeof( serverResponse ) / sizeof( int ); x ++ )
    {
        receivedInt[ x ] = ntohl( serverResponse[ x ] );
    }
    
    printf( "Displaying initial least cost table\n" );
    displayTable( leastCost, sizeof( leastCost ) );
    
    updateTable( leastCost, receivedInt, sizeof( leastCost ), sizeof( receivedInt ) );
    
    printf( "Displaying updated initial least cost table\n" );
    displayTable( leastCost, sizeof( leastCost ) );
    
    if ( n < 0 )
    {
        printf("\n Read error \n");
    }
    
    
    return 0;
}

void displayTable( int leastCost[ ], size_t arraySize )
{

    printf( "Router 0 Destination Router\t\tInterface\t\tLink Cost\n" );
    printf( "%d\t\t\t\t\tLocal\t\t\t0\n", leastCost[ 0 ] );
    for ( int x = 1; x < ( arraySize / sizeof( int ) ) - 1; x += 3 )
    {
        if ( leastCost[ x + 2 ] == 665 )
        {
            printf( "%d\t\t\t\t\tunknown\t\t\tunknown\n", leastCost[ x ] );
        }
        else
        {
            printf( "%d\t\t\t\t\t%d\t\t\t%d\n", leastCost[ x ], leastCost[ x + 1 ], leastCost[ x + 2 ] );
        }
    }
    
}

void updateTable( int leastCost[ ], int receivedLeastCost[ ], size_t leastCostSize, size_t receivedLeastCostSize )
{
    printf( "Updating Router 0 Least Cost Table\n" );
    int interfaceToRouter = 0;
    int costOfRouter = 0;
    
    // Find interface to received router
    for ( int z = 1; z < ( leastCostSize / sizeof( int ) ) -1 ; z+= 3 )
    {
        if ( leastCost[ z ] == receivedLeastCost[ 0 ] )
        {
            interfaceToRouter = leastCost[ z + 1 ];
            costOfRouter = leastCost[ z + 2 ];
            break;
        }
    }

    
    for ( int x = 1; x < ( receivedLeastCostSize / sizeof( int ) ) - 1; x += 3 )
    {
        for ( int y = 1; y < ( leastCostSize / sizeof( int ) ) - 1; y += 3 )
        {
            if ( leastCost[ y ] == receivedLeastCost[ x ] )
            {
                if ( ( receivedLeastCost[ x + 2 ] + costOfRouter ) < leastCost[ y + 2 ] )
                {
                    leastCost[ y + 2 ] = receivedLeastCost[ x + 2 ] + costOfRouter;
                    leastCost[ y + 1 ] = interfaceToRouter;
                }
            }
        }
    }
}