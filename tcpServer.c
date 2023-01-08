#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define PORT 9009

char result[1024];
void serviceClient(int sd);
void concatenate_string(char* s, char* s1)
{   int i;
    int j = strlen(s);
    for (i = 0; s1[i] != '\0'; i++) 
        s[i + j] = s1[i];
    s[i + j] = '\0';
    return; }

int main(int argc, char *argv[]){
int sd, csd, portNumber;
socklen_t len;
struct sockaddr_in servAdd;

char a[3]="000";
int file_disc = open("./counter.txt",O_CREAT| O_RDWR, 0777);
int n=write(file_disc,a,3);
close(file_disc);

if ((sd=socket(AF_INET,SOCK_STREAM,0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
servAdd.sin_family = AF_INET;
servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
servAdd.sin_port = htons(PORT);

bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));

listen(sd, 5);

while(1){
csd=accept(sd,(struct sockaddr*)NULL,NULL);
printf("Got a client\n");
if(fork()==0)
serviceClient(csd);
close(csd);
}
}

const char* command(const char* cmd) {
    char buffer[1024];
    strcpy(result, "");
    FILE* pipe = popen(cmd, "r");
    if (!pipe)
    { printf("popen() failed!");
    exit(1);         
    }
    if (feof(pipe)) return "Not Found!\n";
    
        while (!feof(pipe)) {
             fgets(buffer, 1024, pipe) ;
               concatenate_string(result,buffer); 
                }

    pclose(pipe);
    return result;
}

void serviceClient(int sd){
char message[1024];

while(1){
	if(read(sd, message, 1024)<0){   
close(sd);
fprintf(stderr,"Client is dead, wait for a new client\n");
exit(0);
}

fprintf(stderr, "Client wants : %s\n", message);
command(message);    //calling command function which execute given cmd and return output string wrt that command
write(sd, result, strlen(result)+1);
}
}











