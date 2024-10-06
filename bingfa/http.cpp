#include<iostream>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include <string.h>

const int MAX_SIZE=1024;
const int SMALL_SIZE=100;
void* requst_handle(void* arg);
void send_data( FILE*p,char* st,char* file_name);
void send_error(FILE*p);
char* content_type(char* file);

int main(int argc,char* argv[]){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv,clnt;
    socklen_t clnt_size;
    char buf[MAX_SIZE];
pthread_t id;
    if(argc!=2){
        std::cout<<"error "<<std::endl;
        exit(1);
    }
    
    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&serv,0,sizeof(serv));
    serv.sin_family=AF_INET;
    serv.sin_addr.s_addr=htonl(INADDR_ANY);
    serv.sin_port=htons(atoi(argv[1]));

if(bind(serv_sock,(struct sockaddr*)&serv_sock,sizeof(serv_sock))==-1){
    std::cout<<" bind() error"<<std::endl;
}

if(listen(serv_sock,5)==-1){
    std::cout<<" listen()error"<<std::endl;
}

while(1){
clnt_size=sizeof(clnt);
clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt,&clnt_size);
if(clnt_sock==-1){
    std::cout<<"accept() error"<<std::endl;
    break;
}
std::cout<<"connect clnt_sock :"<<inet_ntoa(clnt.sin_addr)<<" the port"<<ntohs(clnt.sin_port)<<std::endl;
pthread_create(&id,nullptr,requst_handle,(void*)clnt_sock);
pthread_detach(id);
}

    close(serv_sock);
    return 0;
}

void* requst_handle(void* arg){
    int clnt_sock=*((int*)arg);
     char requir_line[SMALL_SIZE];
     FILE* clnt_write;
     FILE* clnt_read;

     char file_name[30];
     char ct[15];
     char methed[10];
     clnt_write=fdopen(clnt_sock,"w");
     clnt_read=fdopen(clnt_sock,"r");
     fgets(requir_line,SMALL_SIZE,clnt_write);//  将传入的消息存入  
     if(strstr(requir_line,"HTTP/")==nullptr){
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return nullptr;
     }

     strcpy(methed,strtok(requir_line," /"));//分割字串 以 /整体为标识符分割字串，并将其变为/0  ,aaa /dddd ded  调用两次,第一次是aaa 第二次没有 /,则以 （空格来切割）就是dddd 
     strcpy(file_name,strtok(nullptr," /"));//第二次调用开始以后 strtok函数第一个参数传递null即可 因为此时定位的起始起点是上一次调用的分割字符串位置的下一个
     strcpy(ct,content_type(file_name));//查看conteny_type
     if(strcmp(methed,"GET")==0){
        send_error(clnt_write);
        fclose(clnt_write);
        fclose(clnt_write);
        return nullptr;
     }

     fclose(clnt_read);
     send_data(clnt_write,ct,file_name);
}

void send_data(FILE* clnt_write,char* ct,char*file_name){
     char protocol[]="HTTP/1.0 200 OK\r\n";
    char server[]="Server:Linux Web Server \r\n";
    char cnt_len[]="Content-length:2048\r\n";
    char cnt_type[SMALL_SIZE];
    char buf[MAX_SIZE];
    FILE* senf;
    senf=fopen(file_name,"r");//打开对应文件 返回文件描述符
    sprintf(cnt_type,"Connect-type:%s\r\n\r\n",ct);
    if(senf==nullptr){
        send_error(clnt_write);
        return ;
    }
    //回应消息 以行为单位传进去
fputs(protocol,clnt_write);
fputs(server,clnt_write);
fputs(cnt_len,clnt_write);
fputs(cnt_type,clnt_write);

while(fgets(buf,MAX_SIZE,senf)!=nullptr){  //用于传输数据 从指定文件读取数据后存入buf  
    fputs(buf,clnt_write);//再由buf转发
    fflush(clnt_write);
}
fflush(clnt_write);
fclose(clnt_write);
//传输完毕
}

char* content_type(char* file){
char extension[SMALL_SIZE];
char file_name[SMALL_SIZE];
strcpy(file_name,file);
strtok(file_name,".");
strcpy(extension,strtok(nullptr,"."));
if(!strcmp(extension,"html")||strcmp(extension,"htm"))//即对应文件有这两个
return "text/html";
else
return "text/plain";
}

void send_error(FILE* p){
    char protocol[]="HTTP/1.0 400 Bad Request\r\n";
    char server[]="Server:Linux Web Server \r\n";
    char cnt_len[]="Content-length:2048\r\n";
    char cnt_type[]="Content-type:text/html\r\n\r\n";
    char content[]="<html><head><title>NETWORK</title></head>"
                   "<body><font size=+5><br>错误！查看请求文件名和请求方式!"
                   "</font></body></html>";

fputs(protocol,p);
fputs(server,p);
fputs(cnt_len,p);
fputs(cnt_type,p);
fflush(p);
}