#include "ui_client.h"

//1.客户注册
int ui_register(int fd)
{
    system("clear");
    MESSAGE Client;
    char passwd[100];
    printf("==================注册界面========================\n\n");   
inputnameerr:    
    printf("请输入用户名:");
    fgets(Client.name,30,stdin);
    you_hua(Client.name);
    if (!check_input_name(Client.name))
    {
        goto inputnameerr;
    }    
inputpwderr:    
    printf("请输入密码  :");
    fgets(Client.msg,30,stdin);
    you_hua(Client.msg);
    printf("请确认密码  :");
    fgets(passwd,30,stdin);
    you_hua(passwd);
    if (!check_register_pwd(Client.msg, passwd))
    {
        goto inputpwderr;
    }
    printf("\n-----------------------------------\n");
    strcpy(Client.flag,"注册"); 
    printf("正在连接服务器，请稍等");
    dynamic_print();
    write(fd,&Client,sizeof(Client));      //向服务器发送信息，请求注册
    return 1;
}

//2.客户端登录
int ui_login(int sockfd)
{ 
    MESSAGE Client;
//	system("clear");
    printf("==================登录界面========================\n\n");
    do {
        printf("\t帐号或者用户名：");
        fgets(Client.name, 30, stdin);
        you_hua(Client.name);
    }while(!check_input_name(Client.name));
//    setbuf(stdin, NULL);
    do {
        printf("\t密          码：");
        fgets(Client.msg, 30, stdin);
        you_hua(Client.msg);
    }while(!check_input_pwd(Client.msg));
    printf("-----------------------------------\n");
//    pnum0=Client.pnum;                            //用全局变量保存登录者的帐号
    strcpy(Client.flag,"登录");
    printf("正在连接服务器，请稍等.\n");
    write(sockfd,&Client,sizeof(Client));        //向服务器发送信息，请求登录
    return 1;
}

//3.登录注册总界面
int ui_register_login(int sockfd)
{
    int iChoice;
    do
    {
        //system("clear");
        printf("===============欢迎进入聊天室==============\n");
        printf("\t1.登录                \n");
        printf("\t2.注册                \n");
        printf("\t0.退出                \n");
        printf("-------------------------------\n");
        printf("请选择:");
        scanf(" %d",&iChoice);//getchar();
        setbuf(stdin,NULL);
    }while((iChoice != 1) && (iChoice != 2) && (iChoice != 0));
    switch(iChoice)
    {
        case 1:
            ui_login(sockfd);
            break;
        case 2:
            ui_register(sockfd);
            break;
        case 0:
            //Exit(sockfd);
            break;
    }
    return iChoice;
}

//4.私聊界面
int ui_private_chat(int sockfd )
{
    return 1;
}

//5.群聊界面
int ui_group_chat(int sockfd)
{
    MESSAGE Client;
    memset(&Client, 0, sizeof(Client));
    while(1)
    {
        printf("all:\n");
        scanf("%s",Client.msg);
        write(sockfd, &Client, sizeof(Client));
    }
    return 1;
}

//6.聊天主界面
int ui_mainchat(int sockfd, MESSAGE *message)
{
    char str[MAXLINE];
    char buf[MAXLINE];
    setbuf(stdin,NULL);
    strcpy(message->flag,"群聊");
    printf("%s您好，如需帮助请输入：help\n",locname);
    while (1)
    {
        printf("%s(), in lines %d!\n", __PRETTY_FUNCTION__, __LINE__);  /********/
        printf("%s:\n", message->flag);
        setbuf(stdin, NULL);
        scanf("%s",str);
        if (1 == help(str))
        {
            continue;
        }
        strcpy(message->name, locname);
        strcpy(buf, message->flag);
        printf("%s(),buf = %s in lines %d!\n", __PRETTY_FUNCTION__, buf, __LINE__);  /********/
        cutStr(str, message->flag, sizeof(str), message->msg, sizeof(str), '#'); //调用字符切割函数
        if(strcmp(message->flag ,"群聊") == 0)      //群聊
        {
            //send(sockfd, message, sizeof(*message), 0);
            write(sockfd, message, sizeof(*message));        //向服务器发送信息
            printf("%s write message to server in lines %d\n", __PRETTY_FUNCTION__, __LINE__);
            continue;
        }

    }

}

//7.主功能界面
int ui_main(int sockfd)
{
    return 1;
}

int Exit(int sockfd)
{
    MESSAGE Client;
    strcpy(Client.flag,"退出");
//	Client.pnum = pnum0;
    write(sockfd,&Client,sizeof(Client));
    close(sockfd);
    exit(0);
}

//功能帮助界面
int help(char str[])
{
    if(strcmp(str, "help") == 0)
    {
        printf("%s 用户，您好：\n",locname);
        printf("all#hello!-------------对所有人说hello!\n");
        printf("用户名#要说的话!---------跟该用户私聊!\n");
        printf("view#------------------查看在线用户\n");
        printf("history#---------------查看聊天记录\n");
        printf("exit#------------------退出\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
