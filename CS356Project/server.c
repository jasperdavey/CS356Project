//
//  server.c
//  CS356Project
//
//  Created by Jasper Davey on 2/26/16.
//  Copyright © 2016 Jasper Davey. All rights reserved.
//

#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main ( int argc, const char* argv[ ] )
{
    int serverSocket = 0;
    int connection = 0;
    struct sockaddr_in serverAddress;
    char sendBuffer[ 1024 ];
    time_t ticks;
    
    serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
    memset( &serverAddress, '0', sizeof( serverAddress ) );
    memset( sendBuffer, '0', sizeof( sendBuffer ) );
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl( INADDR_ANY );
    serverAddress.sin_port = htons( 5000 );
    
    bind( serverSocket, ( struct sockaddr * ) &serverAddress, sizeof( serverAddress ) );
    
    listen( serverSocket, 10 );
    
    while( 1 )
    {
        connection = accept( serverSocket, ( struct sockaddr * ) NULL, NULL );
        
        ticks = time( NULL );
        snprintf( sendBuffer, sizeof( sendBuffer ), "%.24s\r\n", ctime( &ticks ) );
        write( connection, sendBuffer, strlen( sendBuffer ) );
        
        close( connection );
        
        sleep( 1 );
    }
    
    return 0;
}
