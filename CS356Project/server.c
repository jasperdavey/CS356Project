#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
    char receivedString[ 1024 ];
    
    int serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
    
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons( 62742 );
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    bind( serverSocket, ( struct sockaddr * ) &serverAddress, sizeof( struct sockaddr ) );
    
    listen( serverSocket, 4 );
    
    while ( 1 )
    {
        struct sockaddr_in clientAddress;
        socklen_t sin_size = sizeof( struct sockaddr_in );
        int clientSocket = accept( serverSocket, ( struct sockaddr * ) &clientAddress, &sin_size );
        
        recv( clientSocket, receivedString, 500, 0 );
        printf( "Server received: %s\n", receivedString );
        
        close( clientSocket );
    }
    
    return 0;
}