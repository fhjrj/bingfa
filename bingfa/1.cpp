#include<iostream>
#include<sys/socket.h>
#include<pthread.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <string.h>
#include<sys/stat.h>
#include<sys/wait.h>

void* jieshou(void* tag);
int get(char*,int,int);
void unImplemented(int);
void serve_file(int,char*);
void execute_cgi(int,char*,char*,char*);
void not_found(int);
void cat(int,FILE*);
void headers(int,const char*);
void bad_request(int);
 void cannot_execute(int);


void* jieshou(void* tag){
    int clnt_sock=(*(int*)tag);
    char buf[1024];
    char method[256];
    char filename[256];
    char path[256];
    char* idex=nullptr;
    size_t jieshousize;
    int dix=0;
    struct stat st;
    jieshousize=get(buf,clnt_sock,sizeof(buf));
size_t i,j;
while(!isspace(buf[i])&&i<sizeof(method)-1){//isspace函数用于检查是否是空格 如果是返回非零 不是返回零
method[i++]=buf[i++];
}
method[i]='/0';//方法存入
j=i;//继续读请求行

if(strcasecmp(method,"GET")&&strcasecmp(method,"POST")){
    unImplemented(clnt_sock);//HTTP请求没有该请求
    return nullptr;
}

if(strcasecmp(method,"POST")==0)
dix=1;//POST请求中，消息头后面还有信息  GET请求不一定 但是如果GET中有？ 则后面还有信息 需要处理?后面的信息

i=0;
while(isspace(buf[j])&&j<jieshousize){
   j++;
}//跳过所有空格
while(!isspace(buf[j])&&j<sizeof(filename)-1){
    filename[i++]=buf[j++];
}
filename[i]='\0';
//客户端发起请求的url有的带有? 有的没有
if(strcasecmp(method,"GET")==0 ){//GET请求方式
idex=filename;//指向urL 起始位
while((*idex!='?')&&(*idex!='\0'))
idex++;// url中如果有? 则停止在?处 如果没有 则停留在\0处
if(*idex=='?'){
    dix=1;
    *idex='\0';//?前面是网络路径+资源地址
    idex++;
}
}

//客户端请求的url中 域名IP及其前面都是必要的 后面的端口号 文件路径可能被省略
sprintf(path,"htdocs%s",filename);
if (path[strlen(path) - 1] == '/')//省略情况
        strcat(path, "index.html");//追加为完整路径 


    if (stat(path, &st) == -1) {
        while ((jieshousize > 0) && strcmp("\n", buf))  /*获得文件信息失败，将剩下HTTP请求信息存入buf中并且丢弃*/
            jieshousize= get(buf,clnt_sock,sizeof(buf));
        not_found(clnt_sock);
    }
    else
    {
        if ((st.st_mode & S_IFMT) == S_IFDIR)//存在
            strcat(path, "/index.html");
            //如果文件默认有执行权限的，自动解析为cgi程序，如果有执行权限，但不能执行 会报错
        if ((st.st_mode & S_IXUSR) ||
                (st.st_mode & S_IXGRP) ||
                (st.st_mode & S_IXOTH)    )
            dix = 1;
        if (!dix)
            serve_file(clnt_sock, path);//回应客户端的请求且不管HTTP请求体当中消息头的信息（争对的是请求行中无查询参数的，文件无默认权限的，且消息头屁股后面无消息的GET）
        else
            execute_cgi(clnt_sock, path, method,idex);//执行cgi程序（不论是GET还是POST，请求行有？的，消息头屁股后面还有信息或者默认有执行权限的）
    }

    close(clnt_sock);
}


int get(char buf[],int clnt_sock,int size){
int start=0;
int n;
char c='a';
while((start<size-1)&&c!='\n'){  //实际是0~size-1 除去最后一个\0 有效字符是0~size-2
n=read(clnt_sock,&c,1);
if(n==1){
    if(c=='/r'){//遇见空格 不管
        continue;
    }else if(c=='/n'){
        break;//此行读完
    }else{
       buf[start++]=c; 
    }
}else if(n==-1){
    std::cout<<"get-function-read-function  error"<<std::endl;
    start=-1;
    break;
}else if(n==0){
    std::cout<<"clnt close"<<std::endl;
    close(clnt_sock);
    break;
}
}//一行读取完
buf[start]='\0';
return start;
}

void serve_file(int clnt_sock,const char* path){
FILE* fp;
int size=1;
char buf[1024];

buf[0] = 'A'; buf[1] = '\0';
    while ((size > 0) && strcmp("\n", buf))  /* read & discard headers */
        size = get(buf,clnt_sock, sizeof(buf));

fp=fopen(path,"r");
if(fp==nullptr){//未找到
    not_find(clnt_sock);
}else{
    headers(clnt_sock,path);//给客户端回复文件头
    cat(clnt_sock,fp);//文件内容
}
fclose(fp);
}


//执行cgi脚本
void execute_cgi(int clnt_sock,char* path,char* method,char* index){
char buf[1024];
int output[2];
int input[2];
pid_t pid;
int i,status;
int num=1;
int cont_tent=-1;
char c;
buf[0]='A',buf[1]='\0';//确保buf中有东西 可以执行while循环

if(strcasecmp(method,"GET")==0){
while((num>0)&&(strcpy("\n",buf)))// GET模式中 将除请求行，剩下的所有信息存入buf中
num=get(buf,clnt_sock,sizeof(buf));
}else if(strcasecmp(method,"POST")==0){
//POST 先找到 Content-Length

    num=get(buf,clnt_sock,sizeof(buf));//http请求先读取一行 
    while((num>0)&&(strcpy("\n",buf))){
buf[15] = '\0';
            if (strcasecmp(buf, "Content-Length:") == 0)
                cont_tent= atoi(&(buf[16]));
            num= get(buf,clnt_sock,sizeof(buf));
    }
    if(cont_tent==-1){
        bad_request(clnt_sock);
        return ;
    }
}else{//其他请求

}
/*开辟子进程出错 或者开辟管道出错 用于结束错误*/
if (pipe(output) < 0) {
        cannot_execute(clnt_sock);
        return;
    }
    if (pipe(input) < 0) {
        cannot_execute(clnt_sock);
        return;
    }

    if ( (pid = fork()) < 0 ) {
        cannot_execute(clnt_sock);
        return;
    }
    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    write(clnt_sock,buf,strlen(buf));
    if(pid==0){
        char meth_env[255];
        char query_env[255];
        char length_env[255];
         dup2(output[1], 1);//把标准输入重定位到管道写入端 即标准输入后传入管道
        dup2(input[0], 0);//标准输出重定位到管道输出端 即标准输入后 信息从input【0】进行输出
        close(output[0]);
        close(input[1]);
        //配置环境变量
        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);
        if (strcasecmp(method, "GET") == 0) {
            sprintf(query_env, "QUERY_STRING=%s",index );
            putenv(query_env);
        }
        else {   /* POST */
            sprintf(length_env, "CONTENT_LENGTH=%d",index);
            putenv(length_env);
        }
        execl(path, NULL);
        exit(0);
    } else {    /* parent */
        close(output[1]);
        close(input[0]);
        if (strcasecmp(method, "POST") == 0)
            for (i = 0; i <cont_tent; i++) {
                recv(clnt_sock, &c, 1, 0);
                write(input[1], &c, 1);
            }
        while (read(output[0], &c, 1) > 0)
            send(clnt_sock, &c, 1, 0);

        close(output[0]);
        close(input[1]);
        waitpid(pid, &status, 0);

    }

}

//传播消息头
   void headers(int clnt_sock,const char* path){
    char buf[1024];
    (void)clnt_sock;
    strcpy(buf,"HTTP/1.0 200 OK\r\n");
   write(clnt_sock,buf,strlen(buf));
   strcpy(buf,"Server: linuxhttpd/0.1.0\r\n");
   write(clnt_sock,buf,strlen(buf));
   strcpy(buf,"Connect-Type: text/html\r\n");
   write(clnt_sock,buf,strlen(buf));
   strcpy(buf,"\r\n");
   write(clnt_sock,buf,strlen(buf));
   }

   void cat(int clnt_sock,FILE* fp){
    char buf[1024];
    fgets(buf,sizeof(buf),fp);
    while(!feof(fp)){//看文件指针是否到达文件末尾 如果未达到 返回0
        write(clnt_sock,buf,strlen(buf));
        fgets(buf,sizeof(buf),fp);
    }
   }
void not_find(int clnt_sock){
    //未找到目标文档 向客户端回复查找失败
    char buf[1024];
    sprintf(buf,"HTTP/1.0 404 NOT FOUND\r\n");
    send(clnt_sock,buf,strlen(buf),0);
    sprintf(buf,"Server: linuxhttpd/0.1.0\r\n");
    send(clnt_sock,buf,strlen(buf),0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(clnt_sock, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");//剩下的消息
    send(clnt_sock, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    send(clnt_sock, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    send(clnt_sock, buf, strlen(buf), 0);
    sprintf(buf, "your request because the resource specified\r\n");
    send(clnt_sock, buf, strlen(buf), 0);
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    send(clnt_sock, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(clnt_sock, buf, strlen(buf), 0);

}

void unImplemented(int clntsock){
    char buf[1024];
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    write(clntsock, buf, strlen(buf));
    sprintf(buf, "Server: linuxhttpd/0.1.0\r\n");
    write(clntsock, buf, strlen(buf));
    sprintf(buf, "Content-Type: text/html\r\n");
    write(clntsock, buf, strlen(buf));
    sprintf(buf, "\r\n");//消息头已经结尾了
    write(clntsock, buf, strlen(buf));
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    write(clntsock, buf, strlen(buf));
    sprintf(buf, "</TITLE></HEAD>\r\n");
    write(clntsock, buf, strlen(buf));
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    write(clntsock, buf, strlen(buf));
    sprintf(buf, "</BODY></HTML>\r\n");
    write(clntsock, buf, strlen(buf));

}
//that a request has a problem
void bad_request(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(client, buf, sizeof(buf), 0);
}

void cannot_execute(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<P>Error prohibited CGI execution.\r\n");
    send(client, buf, strlen(buf), 0);
}

int main(int argc,char* argv[]){
    struct sockaddr_in ser,clnt;
    int serv_sock,clnt_sock;
    socklen_t size;
    pthread_t t_id;
    if(argc!=2){
        std::cout<<"error"<<std::endl;
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&ser,0,sizeof(ser));
    ser.sin_addr.s_addr=htonl(INADDR_ANY);
    ser.sin_family=AF_INET;
    ser.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&ser,sizeof(ser))==-1){
        std::cout<<"bind()error"<<std::endl;
    }

    if(listen(serv_sock,5)==-1){
        std::cout<<"listen()error"<<std::endl;
    }

    while(1){
        size=sizeof(clnt);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt,&size);
if(clnt_sock==-1){
    std::cout<<"accept()error"<<std::endl;
    break;
}
std::cout<<"connect  "<<inet_ntoa(clnt.sin_addr)<<"the port  "<<ntohs(clnt.sin_port)<<std::endl;
pthread_create(&t_id,nullptr,jieshou,(void*)clnt_sock);
pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}
