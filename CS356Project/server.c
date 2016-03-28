/*  server.c
 *  CS 356 Stage One
 *  Author: Jasper Davey
 */

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

void displayTable( int [ ], size_t );
void updateTable( int [ ], int [ ], size_t, size_t );

int main( int argc, char *argv[ ] )
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    static int leastCost[ 10 ] = { 1, 0, 2, 1, 2, 0, 1, 3, 665, 665 };
    int receivedInt[ 10 ];
    
    listenfd = socket( AF_INET, SOCK_STREAM, 0 );
    memset(&serv_addr, '0', sizeof( serv_addr ) );
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
    serv_addr.sin_port = htons( 5000 );
    
    bind( listenfd, (struct sockaddr* )&serv_addr, sizeof( serv_addr ) );
    
    listen( listenfd, 10 );
    
    while( 1 )
    {
        connfd = accept( listenfd, ( struct sockaddr* ) NULL, NULL );
        
        if ( connfd )
        {

            if ( ( read( connfd, receivedInt, sizeof( receivedInt ) * sizeof( int ) ) ) < 0 )
            {
                printf( "Error reading Least Cost Table\n" );
                return 1;
            }
            
            int receivedArray[ 10 ];
            for ( int x = 0; x < sizeof( receivedArray ) / sizeof( int ); x++ )
            {
                receivedArray[ x ] = ntohl( receivedInt[ x ] );
            }
            
            printf( "Displaying initial least cost table\n" );
            displayTable( leastCost, sizeof( leastCost ) );
            
            updateTable( leastCost, receivedArray, sizeof( leastCost ), sizeof( receivedArray ) );
            
            printf( "Displaying updated initial least cost table\n" );
            displayTable( leastCost, sizeof( leastCost ) );
            
            int sendLeastCost[ 10 ];
            for ( int x = 0; x < sizeof( leastCost ) / sizeof( int ); x++ )
            {
                sendLeastCost[ x ] = htonl( leastCost[ x ] );
            }
            
            printf( "Sending Least Cost Table\n" );
            size_t arraySize = sizeof( sendLeastCost ) * sizeof( sendLeastCost[ 0 ] );
            if ( ( write(connfd, sendLeastCost, arraySize ) ) < 0 )
            {
                printf( "Error sending Least Cost Table\n" );
                return 1;
            }
            
        }
        
        printf( "Least Cost Table sent\n" );
        close( connfd );
        sleep( 1 );
    }
}

void displayTable( int leastCost[ ], size_t arraySize )
{
    
    printf( "Router 1 Destination Router\t\tInterface\t\tLink Cost\n" );
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
    printf( "Updating Router 1 Least Cost Table\n" );
    int interfaceToRouter = 0;
    
    // Find interface to received router
    for ( int z = 1; z < ( leastCostSize / sizeof( int ) ) - 1 ; z+= 3 )
    {
        if ( leastCost[ z ] == receivedLeastCost[ 0 ] )
        {
            printf( "Updated interface\n" );
            interfaceToRouter = leastCost[ z + 1 ];
            break;
        }
    }
    
    printf( "receivedLeastCostSize:%zu\n", receivedLeastCostSize );
    printf( "size of int: %lu\n", sizeof( int ) );
    
    for ( int x = 1; x < ( receivedLeastCostSize / sizeof( int ) ) - 1; x += 3 )
    {
        for ( int y = 1; y < ( leastCostSize / sizeof( int ) ) - 1; y += 3 )
        {
            if ( leastCost[ y ] == receivedLeastCost[ x ] )
            {
                if ( receivedLeastCost[ x + 2 ] < leastCost[ y + 2 ] )
                {
                    leastCost[ y + 2 ] = receivedLeastCost[ x + 2 ];
                    leastCost[ y + 1 ] = interfaceToRouter;
                }
            }
        }
    }
}