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
    int sock, connected, bytes_recieved , true = 1;
    char send_data [1024] , recv_data[1024];
    
    struct sockaddr_in server_addr,client_addr;
    int sin_size;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    // setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero),8);
    printf("%d\n",sock);
    bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    
    listen(sock, 5);
    printf("\nTCPServer Waiting for client on port 5000");
    fflush(stdout);
    int count = 0,i;
    char a[20][10],d[20][10];
    strcpy(a[0],"A");
    strcpy(a[1],"B");
    strcpy(a[2],"C");
    strcpy(a[3],"D");
    strcpy(d[0],"123");
    strcpy(d[1],"124");
    strcpy(d[2],"100");
    strcpy(d[3],"99");
    
    while(1)
    {
        
        sin_size = sizeof(struct sockaddr_in);
        
        connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);
        
        printf("\n I got a connection from (%s , %d)",
               inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        
        while (1)
        {
            ++count;
            printf("\n SEND : ");
            //gets(send_data);
            
            if(count == 1)
                strcpy(send_data,"Welcome to TCPServer");
            send(connected, send_data,strlen(send_data), 0);
            
            bytes_recieved = recv(connected,recv_data,1024,0);
            
            recv_data[bytes_recieved] = '\0';
            
            printf("\n RECIEVED DATA = %s " , recv_data);
            int p = 0;
            for(i=0;i<4;i++)
            {
                if(strcmp(recv_data,d[i]) == 0)
                {
                    strcpy(send_data,a[i]);p=1;
                }
            }
            if(p == 0)
                strcpy(send_data,"No one on that role.");
            fflush(stdout);
        }
    }
    
    close(sock);
    return 0;
}