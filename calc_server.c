#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);
int calculate(int opcnt,int *operand,char operator);
int main(int argc,char *argv[]){
    int serv_sock;
    int clnt_sock;

    char message[BUF_SIZE];
    int str_len,i;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size; 



    if(argc!=2){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0 ,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind() error");
    
    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");
    
    clnt_addr_size=sizeof(clnt_addr);

    for(i=0 ; i<5 ; i++)
    {
        int recv_len=0;
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock==-1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n",i+1);

        char opcnt;
        read(clnt_sock, &opcnt, 1);
    
        printf("%d\n",opcnt);
        recv_len=0;
        while((str_len = read(clnt_sock, message+recv_len, BUF_SIZE))!=0){
            recv_len+=str_len;
            printf("%d\n",str_len);
            if(recv_len==4*opcnt+1)break;
        }
       
        printf("!\n");
        int res=calculate(opcnt,(int*)message,message[recv_len-1]);
        printf("%d",res);
        write(clnt_sock,&res,sizeof(res));
        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

int calculate(int opcnt,int *operand,char operator){
    int res=operand[0];
    printf("%c!\n",operator);
    for(int i=1 ; i<opcnt ; i++){
        if(operator=='+')
            res+=operand[i];
        if(operator=='-')
            res-=operand[i];
        if(operator=='*')
            res*=operand[i];
    }
    return res;
}
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}