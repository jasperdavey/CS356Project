//
//  client.c
//  CS356Project
//
//  Created by Jasper Davey on 2/26/16.
//  Copyright © 2016 Jasper Davey. All rights reserved.
//
//  Router 0

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main( int argc, const char * argv[ ] )
{
    
    int clientSocket;
    int distanceVectorRoutingTable[50];
    static int routingNumber = 0;
    char* serverIP = "10.80.0.11";
    int reply;
    char receiveBuffer[ 1024 ];
    struct sockaddr_in serverAddress;
    
    if ( ( clientSocket = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        printf( "Error creating socket\n" );
        return 1;
    }
    
    memset( &serverAddress, '0', sizeof( serverAddress ) );
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);
    
    if ( inet_pton( AF_INET, serverIP, &serverAddress.sin_addr ) <= 0 )
    {
        printf( "Error connecting with inet_pton\n" );
        return 1;
    }
    
    if ( connect( clientSocket, ( struct sockaddr * ) &serverAddress, sizeof( serverAddress ) ) < 0 )
    {
        printf( "Failed to connect" );
        return 1;
    }

    while ( ( reply = read( clientSocket, receiveBuffer, sizeof( receiveBuffer ) - 1 ) ) > 0 )
    {
        receiveBuffer[ reply ] = 0;
        if ( fputs( receiveBuffer, stdout ) == EOF )
        {
            printf( "Error with fputs\n" );
        }
    }
    
    if ( reply < 0 )
    {
        printf( " Read error\n" );
    }
    
    return 0;
}
