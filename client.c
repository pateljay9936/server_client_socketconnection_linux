#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 9010
char IP1[2]= "i6";
char IP2[13]= "192.168.2.162";

int client_no()
{   int n,count;
    char a[3];
    int file_disc = open("./counter.txt",O_CREAT| O_RDWR, 0777);
    n=read(file_disc,a,3);
    count = atoi(a);
    count++;
    sprintf(a, "%d", count);  
    lseek(file_disc,0,SEEK_SET);
    n=write(file_disc,a,3);
    printf("Client no: %c \n",a[0]);
    return count;
    close(file_disc);
}

int main(int argc, char *argv[]){
char message[1024];

int ssd, portNumber;
socklen_t len;
struct sockaddr_in servAdd;

int counter = client_no();   //function will return the current running client number

if((ssd = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}

servAdd.sin_family = AF_INET;
servAdd.sin_port = htons(PORT);


        if(counter < 6)
        {
            if(inet_pton(AF_INET,IP1,&servAdd.sin_addr)<0){
            fprintf(stderr, " inet_pton() has failed\n");
            exit(2);
            }
        }//server A
        else if(counter < 11)
        {
            if(inet_pton(AF_INET,IP2,&servAdd.sin_addr)<0){
            fprintf(stderr, " inet_pton() has failed\n");
            exit(2);
            }
        }//server B
        else if(counter % 2 == 1)
        {
            if(inet_pton(AF_INET,IP1,&servAdd.sin_addr)<0){
            fprintf(stderr, " inet_pton() has failed\n");
            exit(2);
            }
        }//server A
        else
        {
            if(inet_pton(AF_INET,IP2,&servAdd.sin_addr)<0){
            fprintf(stderr, " inet_pton() has failed\n");
            exit(2);
            }
        }//server B

if(connect(ssd,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
fprintf(stderr, "connect() has failed, exiting\n");
exit(3);
}



while(1){



fprintf(stderr, "Enter the 'quit' to quit or a message for the server\n");
fgets(message, 1023, stdin);
if(message[0] == 'q'&& message[1] == 'u' && message[2] == 'i' && message[3] == 't'){
close(ssd);
exit(0);
}
write(ssd, message, strlen(message)+1);

strcpy(message, "");
if(!read(ssd, message, 1024)){
fprintf(stderr, "read() error\n");
exit(3);
}
printf("server's message:\n %s\n",message);
//fprintf(stderr, "server's message:\n %s\n",message);

}
}










