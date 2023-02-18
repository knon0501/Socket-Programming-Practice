#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc,char *argv[]){
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_addr;
    int opcnt;
    int operand[BUF_SIZE];
    char operator;

    int str_len,recv_len,recv_cnt;

    if(argc!=3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM,0);

    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0 ,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("connect) error!");
    else
        puts("connected..........");

    fputs("Operand count: ",stdout);
    scanf("%d",&opcnt);

    for(int i=0 ; i<opcnt ; i++){
        printf("Operand %d: ",i+1);
        scanf("%d",&operand[i]);
    }
    printf("Operator :");
    scanf(" %c",&operator);
   // printf("%c\n",operator);
    char op_cnt=opcnt;
    write(sock,&op_cnt,1);
    write(sock,(char*)operand,4*opcnt);
    write(sock,&operator,1);

    int res;
    read(sock,&res,4);
    printf("Operation result: %d",res);
    close(sock);

    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}