#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER 1000

void DieWithError(char *error){
 perror(error);
 exit(1);
}


int main(int argc, char* argv[]){

int client_socket;
int serv_socket;
 struct sockaddr_in serv_addr;//server address

  int status;
 int bytes_received,msglen,totalBytesRcv=0;
 char echo_buffer[BUFFER],receive[BUFFER],temp[BUFFER];//message to send

 /*if(argc<3||argc>4){
  DieWithError("Unknown number of arguments");
 }*/
  if((serv_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
   DieWithError("socket() error");
  }
  bzero(&serv_addr,sizeof(serv_addr));//set structure to zero

  serv_addr.sin_family=AF_INET ;// IP protocol family
  serv_addr.sin_addr.s_addr=inet_addr(argv[1]);//IP address
  serv_addr.sin_port=htons(atoi(argv[2]));// server port


   bind(serv_socket,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

//establish connection to the server

 if(connect(serv_socket,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
    DieWithError("connect() error");
    }//connect to the server address


   while(1){
  bzero(echo_buffer,BUFFER);
  bzero(receive,BUFFER);
  if(recv(serv_socket,receive,BUFFER,0)<0){
    DieWithError("recv() failed or connection closed prematurely");
   }
   printf("%s",receive);//print the string


 fgets(echo_buffer,BUFFER,stdin);
 if(strcmp(echo_buffer,"quit\n")==0){
     exit(0);
   }
   strcpy(temp,argv[3]);
   strcat(temp,": ");
   strcat(temp,echo_buffer);
   strcpy(echo_buffer,temp);
 if(send(serv_socket,echo_buffer,BUFFER,0)<0){
  DieWithError("send() sent a different number of bytes than expected");
 }

  }

  //close(serv_socket);
  //shutdown(serv_socket,SHUT_RDWR);
  close(serv_socket);
  return 0;

}
